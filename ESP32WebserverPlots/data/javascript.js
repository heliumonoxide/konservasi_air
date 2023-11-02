// var array_length = 5;  // final_quality, temperature_value, ph_value, ntu_value, tds_value (Currently only for 5 sensors on 1 Niagara place)
// var sensorsValue = [];
// for (let i = 0; i < array_length; i++) {
//   sensorsValue[i] = i;
// }

// var final_quality = document.getElementById('final_quality');
// var temperature_value = document.getElementById('temperature_value');
// var ph_value = document.getElementById('ph_value');
// var ntu_value = document.getElementById('ntu_value');
// var tds_value = document.getElementById('tds_value');

// var slider = document.getElementById('ID_RANDOM_INTENSITY');
// slider.addEventListener('change', slider_changed);

var Socket;

function init() {
  Socket = new WebSocket('ws://' + window.location.hostname + ':81/');
  Socket.onmessage = function(event) {
    processCommand(event);
  };
}

// function slider_changed () {
//   var l_random_intensity = slider.value;
//   console.log(l_random_intensity);
//   var msg = { type: "random_intensity", value: l_random_intensity};
//   Socket.send(JSON.stringify(msg)); 
// }

function processCommand(event) {
  var obj = JSON.parse(event.data);
  var type = obj.type;
  if (type.localeCompare("dashboard_update") == 0) {
    console.log(obj.value);
    // final_quality.innerHTML = obj.value[0];
    // temperature_value.innerHTML = obj.value[1];
    // ph_value.innerHTML = obj.value[2];
    // ntu_value.innerHTML = obj.value[3];
    // tds_value.innerHTML = obj.value[4];
    if (obj.value[0] >= 7) {
      document.getElementById('final_quality').innerHTML = 'GOOD';
    } else if (obj.value[0] >= 4 && obj.value[0] < 7) {
      document.getElementById('final_quality').innerHTML = 'OK';
    } else if (obj.value[0] < 4) {
      document.getElementById('final_quality').innerHTML = 'BAD';
    }
    document.getElementById('temperature_value').innerHTML = obj.value[1];
    document.getElementById('ph_value').innerHTML = obj.value[2];
    document.getElementById('ntu_value').innerHTML = obj.value[3];
    document.getElementById('tds_value').innerHTML = obj.value[4];
    // myChart.data.datasets[0].data = obj.value;
    // myChart.update();
  }
  // else if (type.localeCompare("random_intensity") == 0) { 
  //   var l_random_intensity = parseInt(obj.value); 
  //   console.log(l_random_intensity); 
  //   slider.value = l_random_intensity; 
  //   output.innerHTML = l_random_intensity;
  // }
}
window.onload = function(event) {
  init();
}