var i = 0;
var i2 = 0;
var i3 = 0;
var width_ph_bar = 0;
var width_ntu_bar = 0;
var width_tds_bar = 0;

// Map function to convert the range
function mapRange(value, inMin, inMax, outMin, outMax) {
	return ((value - inMin) * (outMax - outMin)) / (inMax - inMin) + outMin;
 }

function adjust_bar_ph() {
  if (i == 0) {
    i = 1;
    var elem1 = document.getElementById("pHBar");

	// var pHValue = Math.floor((Math.random() * 14)); // Dicomment setelah udah fix

	var pHValue = document.getElementById('ph_value').innerHTML;
	// document.getElementById('ph_value').innerHTML = pHValue; // Dicomment setelah udah fix

	var value = Math.floor(mapRange(pHValue, 0, 14, 0, 100));

	id = setInterval(bar_ph_adjust, 10);

	function bar_ph_adjust(){
		if(width_ph_bar == value){
			i = 0;
			clearInterval(id);
		}
		else if(width_ph_bar < value){
			width_ph_bar++;
			elem1.style.width = width_ph_bar + "%";
		}
		else if(width_ph_bar > value){
			width_ph_bar--;
			elem1.style.width = width_ph_bar + "%";
		}
	}
  }
}

function adjust_bar_ntu() {
	if (i2 == 0) {
	  i2 = 1;
	  var elem2 = document.getElementById("NTUBar");

	//   var NTUValue = Math.floor((Math.random() * 3000)); // Dicomment setelah udah fix
  
	  var NTUValue = document.getElementById('ntu_value').innerHTML; 
	//   document.getElementById('ntu_value').innerHTML = NTUValue; // Dicomment setelah udah fix
  
	  var value2 = Math.floor(mapRange(NTUValue, 0, 3000, 0, 100));
  
	  id2 = setInterval(bar_ntu_adjust, 10);
  
	  function bar_ntu_adjust(){
		  if(width_ntu_bar == value2){
			  i2 = 0;
			  clearInterval(id2);
		  }
		  else if(width_ntu_bar < value2){
			  width_ntu_bar++;
			  elem2.style.width = width_ntu_bar + "%";
		  }
		  else if(width_ntu_bar > value2){
			  width_ntu_bar--;
			  elem2.style.width = width_ntu_bar + "%";
		  }
		}
	}
  }


  function adjust_bar_tds() {
	if (i3 == 0) {
	  i3 = 1;
	  var elem3 = document.getElementById("TDSBar");

	//   var TDSValue = Math.floor((Math.random() * 1000)); // Dicomment setelah udah fix
	  var TDSValue = document.getElementById('tds_value').innerHTML;

	//   document.getElementById('tds_value').innerHTML = TDSValue; // Dicomment setelah udah fix
	  var value3 = Math.floor(mapRange(TDSValue, 0, 1000, 0, 100));
  
	  id3 = setInterval(bar_tds_adjust, 10);
  
	  function bar_tds_adjust(){
		  if(width_tds_bar == value3){
			  i3 = 0;
			  clearInterval(id3);
		  }
		  else if(width_tds_bar < value3){
			  width_tds_bar++;
			  elem3.style.width = width_tds_bar + "%";
		  }
		  else if(width_tds_bar > value3){
			  width_tds_bar--;
			  elem3.style.width = width_tds_bar + "%";
		  }
	  }
	}
  }

// setInterval(adjust_bar_ph, 2000); // Dicomment setelah udah fix
// setInterval(adjust_bar_ntu, 2000); // Dicomment setelah udah fix
// setInterval(adjust_bar_tds, 2000); // Dicomment setelah udah fix