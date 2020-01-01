let x = [-86,-114,-106,106,107,-111,133,221,-783,2478];
let y = [86,114,106,106,107,111,133,221,783,-2478];

let min_x = Math.min( ...x ),
    max_x = Math.max( ...x );
max_y = Math.max( ...y );
min_y = Math.max( ...y );

new Chart(document.getElementById("myChart"), {
    type: 'line',
    data: {
        labels: x,
        datasets: [{
            data: y,
            label: "y = f(x)",
            borderColor: "#3e95cd",
            fill: false
        }
        ]
    },
    options: {
        title: {
            display: true,
            text: 'function y = f(x)'
        },
        scales: {
            yAxes:[{
                stacked:true,
                gridLines: {
                    display:true,
                    color:"rgba(255,99,132,0.2)"
                },
                ticks: {
                    suggestedMax: max_y,
                    suggestedMin: min_y,
                }
            }],
            xAxes:[{
                stepSize: 100,
                stacked: true,
                gridLines: {
                    display:true
                },
                ticks: {
                    suggestedMax: max_x,
                    suggestedMin: min_x,
                }
            }]
        }
    }
});