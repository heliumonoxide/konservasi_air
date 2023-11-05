var i = 0;

// Map function to convert the range
function mapRange(value, inMin, inMax, outMin, outMax) {
	return ((value - inMin) * (outMax - outMin)) / (inMax - inMin) + outMin;
 }

function adjust_bar() {
  if (i == 0) {
    i = 1;
    var elem = document.getElementById("pHBar");
    var width = 0;
	id = setInterval(bar_adjust, 10);
	// var pHValue = Math.floor((Math.random() * 14)); // Harus diganti dengan data pH Asli
	var pHValue = document.getElementById('ph_value').innerHTML;
	var value = mapRange(pHValue, 0, 14, 0, 100);
	function bar_adjust(){
		if(width >= value){
			i = 0;
			clearInterval(id);
		}
		else{
			width++;
			elem.style.width = width + "%";
		}
	}
  }
}

setInterval(adjust_bar, 2000); // Kalo misalnya udah stabil, Ini hapus, masukin ke event ngambil data