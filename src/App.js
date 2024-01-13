import Container from 'react-bootstrap/Container';
import Row from 'react-bootstrap/Row';
import Col from 'react-bootstrap/Col';
import './App.css';
import Questionnaire from './Questionnaire';
import 'bootstrap/dist/css/bootstrap.css';

function App() {
  return (
    <Container className="App">
        <Row>
            <Col></Col>
            <Col>
                <h1 className="title">Dating Questionnaire</h1>
                <Questionnaire></Questionnaire>
            </Col>
            <Col></Col>
        </Row>
    </Container>
  );
}

export default App;
