document.getElementById('numProcesses').addEventListener('change', generateProcessInputs);
document.getElementById('algorithm').addEventListener('change', generateQuantumInput);
document.getElementById('processForm').addEventListener('submit', handleSubmit);

function generateProcessInputs() {
    const numProcesses = document.getElementById('numProcesses').value;
    const processInputs = document.getElementById('processInputs');
    processInputs.innerHTML = '';
    
    for (let i = 0; i < numProcesses; i++) {
        processInputs.innerHTML += `
            <div class="form-group">
                <label for="arrival${i}">Process ${i + 1} Arrival Time</label>
                <input type="number" id="arrival${i}" class="form-control" required>
                <label for="burst${i}" class="mt-2">Process ${i + 1} Burst Time</label>
                <input type="number" id="burst${i}" class="form-control" required>
                <label for="priority${i}" class="mt-2">Process ${i + 1} Priority</label>
                <input type="number" id="priority${i}" class="form-control" required>
            </div>
        `;
    }
}

function generateQuantumInput() {
    const algorithm = document.getElementById('algorithm').value;
    const quantumInput = document.getElementById('quantumInput');

    if (algorithm === 'roundrobin') {
        quantumInput.innerHTML = `
            <div class="form-group">
                <label for="quantum">Quantum Time</label>
                <input type="number" id="quantum" class="form-control" required>
            </div>
        `;
    } else {
        quantumInput.innerHTML = '';
    }
}

async function handleSubmit(event) {
    event.preventDefault();
    
    const numProcesses = document.getElementById('numProcesses').value;
    const algorithm = document.getElementById('algorithm').value;
    let processes = [];
    let quantum = null;

    if (algorithm === 'roundrobin') {
        quantum = parseInt(document.getElementById('quantum').value);
    }

    for (let i = 0; i < numProcesses; i++) {
        const arrival = parseInt(document.getElementById(`arrival${i}`).value);
        const burst = parseInt(document.getElementById(`burst${i}`).value);
        const priority = parseInt(document.getElementById(`priority${i}`).value);
        processes.push({ id: i + 1, arrival, burst, priority });
    }

    const response = await fetch(`http://localhost:8080/${algorithm}`, {
        method: 'POST',
        headers: {
            'Content-Type': 'application/json',
        },
        body: JSON.stringify({
            processes: processes,
            quantum: quantum,
        }),
    });

    const result = await response.json();
    displayResult(result);
}

function displayResult(result) {
    const resultDiv = document.getElementById('result');
    resultDiv.innerHTML = '';

    const processes = result.processes;
    resultDiv.innerHTML += `<h3>Process Scheduling Result</h3>`;
    resultDiv.innerHTML += `<table class="table table-bordered"><thead><tr><th>Process ID</th><th>Arrival Time</th><th>Burst Time</th><th>Priority</th><th>Waiting Time</th><th>Turnaround Time</th></tr></thead><tbody>`;

    processes.forEach(p => {
        resultDiv.innerHTML += `
            <tr>
                <td>${p.id}</td>
                <td>${p.arrival}</td>
                <td>${p.burst}</td>
                <td>${p.priority}</td>
                <td>${p.waiting}</td>
                <td>${p.turnaround}</td>
            </tr>
        `;
    });

    resultDiv.innerHTML += `</tbody></table>`;
}
