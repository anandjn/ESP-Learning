var connection = new WebSocket('ws://'+location.hostname + ':81/', ['arduino']);

var chartBox = document.getElementById('myChart');
var myChart = new Chart(chartBox, {
    type: 'bar',
    data: {
        labels: ['63Hz', '160Hz', '400Hz', '1kHz', '2.5kHz', '6.25kHz', '16kHz'],
        datasets:[{
            label:'amplitude of frequency',
            data: [],
            backgroundColor : "#555"
        }]
    },
    options: {
        responsive: false,
        animation: false,
        scales: {
            yAxes: [{
                ticks: {
                    beginAtZero: true,
                    max: 1023
                }
            }]
        }
    }
    
});

connection.onopen = function () {
    connection.send('Connect' + new Date());
};

connection.onerror = function () {
    console.log('WebSocket Error ', error);
};

connection.onmessage = function (event) {
    console.log('Server ', event.data);
    data = JSON.parse(event.data);
    myChart.data.datasets[0].data= [];
    
    data.forEach(function(element) {
       myChart.data.datasets[0].data.push(element.value); 
    });
    
    myChart.update();         
};

connection.onclose = function () {
    console.log('WebSocket connection closed');  
};

function sendSig() {
    var r = document.getElementById('r').value;
    var y = document.getElementById('y').value;
    var packet = "#" + r.toString() + " " + y.toString(); 
    console.log(packet);
    connection.send(packet);
}