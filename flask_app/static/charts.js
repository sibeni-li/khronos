const container = document.getElementById("container-charts");
const barContainer = document.getElementById("bar-chart");
const pieContainer = document.getElementById("pie-chart");

const functionsData = JSON.parse(container.dataset.functions);

let exec = [];
let call = [];
let fctName = [];

for (let i = 0; i < functionsData.length; i++) {
    exec.push(functionsData[i]["exec_time"])
    call.push(functionsData[i]["call_count"])
    fctName.push(functionsData[i]["name"])
}

let trace1 = {
    x: fctName,
    y: exec,
    name: "Execution time per function",
    type: "bar"
};

let trace2 = {
    x: fctName,
    y: call,
    name: "Call count per function",
    type: "bar"
};

let data = [trace1, trace2];
let layout = {
    font: {
        color: '#ffffff'
    },
    width: 500,
    paper_bgcolor: '#212529',
    plot_bgcolor: '#212529',
    barmode: "group"
};

Plotly.newPlot(barContainer, data, layout);

let pieData = [{
    values: exec,
    labels: fctName,
    type: "pie"
}];

let pieLayout = {
    font: {
        color: '#ffffff'
    },
    paper_bgcolor: '#212529',
    height: 400,
    width: 500
};

Plotly.newPlot(pieContainer, pieData, pieLayout)
