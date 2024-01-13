import json from './questions.json';
import Form from 'react-bootstrap/Form';

function MCQuestions() {
    const questions = [];
    for (const question of json) {
        questions.push(
            <li>
                <Form.Group className="mb-3">
                    <Form.Label>{question.label}</Form.Label>
                    {question.options.map((opt, i) => {
                        return (
                            <Form.Check
                                required
                                type="radio"
                                name={question.name}
                                value={i}
                                label={opt}
                            />
                        );
                    })}
                </Form.Group>
            </li>
        );
    }
    return (
        <ol start="0">
            {questions}
        </ol>
    );
}

export default MCQuestions;
