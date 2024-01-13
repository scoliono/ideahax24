import json from './questions.json';
import Form from 'react-bootstrap/Form';

function oneHot(i, choices) {
    let str = [];
    for (let j = 0; j < choices.length; ++j) {
        str.push("0");
    }
    str[i] = "1";
    return str.join("");
}

function MCQuestions() {
    const questions = [];
    for (const question of json) {
        questions.push(
            <li key={question.name} className="m-auto">
                <Form.Group className="m-auto mb-5 w-50 text-left">
                    <Form.Label>{question.label}</Form.Label>
                    {question.options.map((opt, i) => {
                        return (
                            <Form.Check
                                key={`${question.name}_choice_${i}`}
                                required
                                type="radio"
                                name={question.name}
                                value={oneHot(i, question.options)}
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
