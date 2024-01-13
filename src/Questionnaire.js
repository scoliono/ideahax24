import Container from 'react-bootstrap/Container';
import Row from 'react-bootstrap/Row';
import Col from 'react-bootstrap/Col';
import Form from 'react-bootstrap/Form';
import Button from 'react-bootstrap/Button';
import MCQuestions from './MCQuestions';

function BirthdayDateSelect() {
    const options = [];
    for (let i = 1; i <= 31; ++i) {
        options.push(<option value={i}>{i}</option>);
    }
    return options;
}

function BirthdayYearSelect() {
    const options = [];
    for (let i = 0; i < 50; ++i) {
        options.push(<option value={2006 - i}>{2006 - i}</option>);
    }
    return options;
}

function sendFormData(event) {
    event.preventDefault();
    const form = document.getElementById("questionnaire");
    const formData = new FormData(form);
    console.log(formData);
}

function Questionnaire() {
    return (
        <Form onSubmit={sendFormData} id="questionnaire">
            <Form.Group className="mb-3" controlId="name">
                <Form.Label>Name</Form.Label>
                <Form.Control placeholder="John Smith" required maxLength={32} />
            </Form.Group>
            <Form.Label>Birthday</Form.Label>
            <Container className="mb-3">
                <Row>
                    <Col>
                        <Form.Select id="bdayMonth">
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
                        <Form.Select id="bdayDate">
                            <BirthdayDateSelect></BirthdayDateSelect>
                        </Form.Select>
                    </Col>
                    <Col>
                        <Form.Select id="bdayYear">
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
                        value="M"
                        name="gender"
                    />
                    <Form.Check
                        inline
                        required
                        type="radio"
                        label="Female"
                        value="F"
                        name="gender"
                    />
                    <Form.Check
                        inline
                        required
                        type="radio"
                        label="Other"
                        value="N"
                        name="gender"
                    />
                </div>
            </Form.Group>
            <Form.Group className="mb-3">
                <Form.Label>
                    Looking for
                </Form.Label>
                <div>
                    <Form.Check
                        inline
                        type="checkbox"
                        label="Male"
                        value="M"
                        name="desiredGender"
                    />
                    <Form.Check
                        inline
                        type="checkbox"
                        label="Female"
                        value="F"
                        name="desiredGender"
                    />
                    <Form.Check
                        inline
                        type="checkbox"
                        label="Other"
                        value="N"
                        name="desiredGender"
                    />
                </div>
            </Form.Group>
            <MCQuestions />
            <Button type="submit">Submit</Button>
        </Form>
    );
}

export default Questionnaire;
