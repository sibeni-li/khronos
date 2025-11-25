// Get container elements
const container = document.getElementById("container-charts");
const barContainer = document.getElementById("bar-chart");
const pieContainer = document.getElementById("pie-chart");

// Parse function data from data attribute
const functionsData = JSON.parse(container.dataset.functions);

// Initialize arrays to hold charts data
let exec = [];
let call = [];
let fctName = [];

// Extract data from functions array into separate arrays for plotting
for (let i = 0; i < functionsData.length; i++) {
    exec.push(functionsData[i]["exec_time"])
    call.push(functionsData[i]["call_count"])
    fctName.push(functionsData[i]["name"])
}

// Define first trace for bar chart
let trace1 = {
    x: fctName,
    y: exec,
    name: "Execution time per function",
    type: "bar"
};

// Define second trace for bar chart
let trace2 = {
    x: fctName,
    y: call,
    name: "Call count per function",
    type: "bar"
};

// Combine traces for grouped bar chart
let data = [trace1, trace2];

// Configure bar chart layout
let layout = {
    font: {
        color: '#ffffff'
    },
    width: 500,
    paper_bgcolor: '#212529',
    plot_bgcolor: '#212529',
    barmode: "group"
};

// Render bar chart
Plotly.newPlot(barContainer, data, layout);

// Define pie chart data
let pieData = [{
    values: exec,
    labels: fctName,
    type: "pie"
}];

// Configure pie chart layout
let pieLayout = {
    font: {
        color: '#ffffff'
    },
    paper_bgcolor: '#212529',
    height: 400,
    width: 500
};

// Render pie chart
Plotly.newPlot(pieContainer, pieData, pieLayout)
