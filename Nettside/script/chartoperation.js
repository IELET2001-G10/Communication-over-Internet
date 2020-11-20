var now = new Date();
var time_now = now.getHours() + ':' + now.getMinutes() + ':' + now.getSeconds();

var ctx = document.getElementById('myChart').getContext('2d'); //Defines the basic graphic element of the graph

var myLineChart = new Chart(ctx, { //Defines the graph
    type: 'line', //Defines the type of graph
    data: { //Decides how the data (content of the graph will be)
        datasets: [ //Datasets refers to the different graphs and the data they contain
            {
                label: 'Temperatur', //Label of dataset/graph 1
                data: dataArr1, //The dataArray that actually stores the data
                backgroundColor: [ //The background color of the graph (usually not in use)
                    'rgba(255, 99, 132, 0.2)'
                ],
                borderColor: [ //The border color of the graph (the color of the actual line)
                    'rgb(255,12,20)'
                ],
                borderWidth: 1, //The width of the graph line
                fill: false
            },
            {
                label: 'Trykk',
                data: dataArr2,
                backgroundColor: [
                    'rgba(255, 99, 132, 0.2)'
                ],
                borderColor: [
                    'rgb(70,255,28)'
                ],
                borderWidth: 1,
                fill: false
            },
            {
                label: 'Fuktighet', //Label of dataset/graph 1
                data: dataArr3, //The dataArray that actually stores the data
                backgroundColor: [ //The background color of the graph (usually not in use)
                    'rgba(255, 99, 132, 0.2)'
                ],
                borderColor: [ //The border color of the graph (the color of the actual line)
                    'rgb(12,73,255)'
                ],
                borderWidth: 1, //The width of the graph line
                fill: false
            },
            {
                label: 'Luftkvalitet', //Label of dataset/graph 1
                data: dataArr4, //The dataArray that actually stores the data
                backgroundColor: [ //The background color of the graph (usually not in use)
                    'rgba(255, 99, 132, 0.2)'
                ],
                borderColor: [ //The border color of the graph (the color of the actual line)
                    'rgb(210,12,255)'
                ],
                borderWidth: 1, //The width of the graph line
                fill: false
            }
        ]
    },
    options: {
        scales: {
            xAxes: [{
                scaleLabel: {
                    display: true,
                    labelString: 'tid'
                }
            }],
            yAxes: [{
                ticks: {
                    beginAtZero: true //Keep this true to begin at zero in the graph
                }
                scaleLabel: {
                    display: true,
                    labelString: 'verdi'
                }
            }]
        },
        responsive: true,
        maintainAspectRatio: false
    }
});
