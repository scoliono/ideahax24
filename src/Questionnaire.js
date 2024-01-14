import Container from 'react-bootstrap/Container';
import Row from 'react-bootstrap/Row';
import Col from 'react-bootstrap/Col';
import Form from 'react-bootstrap/Form';
import Button from 'react-bootstrap/Button';
import MCQuestions from './MCQuestions';

function BirthdayDateSelect() {
    const options = [];
    for (let i = 1; i <= 31; ++i) {
        options.push(<option key={`bday_date_${i}`} value={i}>{i}</option>);
    }
    return options;
}

function BirthdayYearSelect() {
    const options = [];
    for (let i = 0; i < 50; ++i) {
        options.push(<option key={`bday_year_${i}`} value={2006 - i}>{2006 - i}</option>);
    }
    return options;
}

const usbProductId = 60000;
const usbVendorId = 4292;

async function setupPort() {
    if (!navigator.serial) {
        alert("Your browser does not have the Web Serial API enabled, please use Chrome and enable it in chrome://flags");
        return false;
    }
    const port = await navigator.serial.requestPort({ filters: [{ usbProductId, usbVendorId }] });
    try {
        await port.open({ baudRate: 9600 });
    } catch (err) {
        // DOMException if port is open is normal
        if (!(err instanceof DOMException)) {
            console.error(err);
        }
    }
    return port;
}

async function sendFormData(event) {
    event.preventDefault();
    const form = document.getElementById("questionnaire");
    const formData = new FormData(form);

    // name is not part of similarity
    let name = formData.get("name");
    formData.delete("name");
    // gender is non-negotiable
    const gender = Number(formData.get("gender"));
    formData.delete("gender");
    // relationship goal is non-negotiable
    const goal = Number(formData.get("goal"));
    formData.delete("goal");
    // desired gender handled as bitmask
    const desiredGenderVec = formData.getAll("desiredGender");
    formData.delete("desiredGender");
    const desiredGender = desiredGenderVec.reduce((prev, val) => {
        return prev |= Number(val);
    }, 0);

    // birth year not part of similarity vector
    const bdayYear = Number(formData.get("bdayYear"));
    formData.delete("bdayYear");

    // vector for cosine similarity
    const similarity = [...formData.values()].join("");
    const payload = JSON.stringify({
        name,
        bdayYear,
        gender,
        desiredGender,
        goal,
        similarity
    });
    console.log("Payload:", payload);

    const port = await setupPort();
    if (!(await sendProfileSerial(port, str2ab(payload)))) {
        alert("Failed to send profile to ESP32, verify that the port is writable");
        return;
    }
    const ack = await readAckSerial(port);
    if (ack && ack === payload) {
        alert("Successfully uploaded profile to ESP32!");
    } else if (ack !== payload) {
        alert("Profile ack from ESP32 did not match!");
    } else {
        alert("Failed to get ack from ESP32");
    }
    await port.close();
}

function str2ab(str) {
    const buf = new ArrayBuffer(str.length);
    const bufView = new Uint8Array(buf);
    for (var i = 0, strLen = str.length; i < strLen; i++) {
        bufView[i] = str.charCodeAt(i);
    }
    return buf;
}

async function sendProfileSerial(port, profile) {
    if (!port.writable) {
        return false;
    }
    const writer = port.writable.getWriter();
    try {
        await writer.write(profile);
        console.log("Serial tx done");
        writer.releaseLock();
    } catch (err) {
        console.error("Serial tx error:", err);
        writer.releaseLock();
        return false;
    }
    return true;
}

async function readAckSerial(port) {
    const reader = port.readable.getReader();
    let ack = "";
    // releasing the lock will stop the read
    setTimeout(() => {
        reader.releaseLock();
    }, 1000);
    try {
        while (true) {
            const { value } = await reader.read();
            const char = String.fromCharCode(value[0]);
            ack += char;
        }
    } catch (err) {
        // TypeError is expected when releasing reader on timeout
        if (!(err instanceof TypeError)) {
            console.error("Serial rx error:", err);
            reader.releaseLock();
        }
    } finally {
        console.log("Serial rx:", ack);
        return ack;
    }
}

function Questionnaire() {
    return (
        <Form onSubmit={sendFormData} id="questionnaire">
            <Form.Group className="mb-3" controlId="name">
                <Form.Label>Name</Form.Label>
                <Form.Control name="name" placeholder="John Smith" required maxLength={32} />
            </Form.Group>
            <Form.Label>Birthday</Form.Label>
            <Container className="mb-3">
                <Row>
                    <Col>
                        <Form.Select>
                            <option value="1">January</option>
                            <option value="2">February</option>
                            <option value="3">March</option>
                            <option value="4">April</option>
                            <option value="5">May</option>
                            <option value="6">June</option>
                            <option value="7">July</option>
                            <option value="8">August</option>
                            <option value="9">September</option>
                            <option value="10">October</option>
                            <option value="11">November</option>
                            <option value="12">December</option>
                        </Form.Select>
                    </Col>
                    <Col>
                        <Form.Select>
                            <BirthdayDateSelect></BirthdayDateSelect>
                        </Form.Select>
                    </Col>
                    <Col>
                        <Form.Select name="bdayYear">
                            <BirthdayYearSelect></BirthdayYearSelect>
                        </Form.Select>
                    </Col>
                </Row>
            </Container>
            <Form.Group className="mb-3">
                <Form.Label>
                    Gender
                </Form.Label>
                <div>
                    <Form.Check
                        inline
                        required
                        type="radio"
                        label="Male"
                        value="1"
                        name="gender"
                    />
                    <Form.Check
                        inline
                        required
                        type="radio"
                        label="Female"
                        value="2"
                        name="gender"
                    />
                    <Form.Check
                        inline
                        required
                        type="radio"
                        label="Other"
                        value="4"
                        name="gender"
                    />
                </div>
            </Form.Group>
            <Form.Group className="mb-5">
                <Form.Label>
                    Looking for
                </Form.Label>
                <div>
                    <Form.Check
                        inline
                        type="checkbox"
                        label="Male"
                        value="1"
                        name="desiredGender"
                    />
                    <Form.Check
                        inline
                        type="checkbox"
                        label="Female"
                        value="2"
                        name="desiredGender"
                    />
                    <Form.Check
                        inline
                        type="checkbox"
                        label="Other"
                        value="4"
                        name="desiredGender"
                    />
                </div>
            </Form.Group>
            <MCQuestions />
            <Button type="submit" className="mb-5">Submit</Button>
        </Form>
    );
}

export default Questionnaire;
