function load_form_values(pagetype)
{
	var form_id;
	if(pagetype != 'bug_report')
				form_id = document.presence_info;
	else
   			form_id = document.bug_report;
      
	form_id.h_device_model.value = device_model;
  	form_id.h_device_model.checked = "1";
   document.getElementById('fld1').innerHTML = "Device model : " + device_model;
   
	form_id.h_firmware_version.value = firmware_version;
  	form_id.h_firmware_version.checked = "1";
   document.getElementById('fld2').innerHTML = "Firmware version : " + firmware_version;

	form_id.h_symbian_version.value = symbian_version;
  	form_id.h_symbian_version.checked = "1";
   document.getElementById('fld3').innerHTML = "Symbian version : " + symbian_version;

	form_id.h_s60_version.value = s60_version;
  	form_id.h_s60_version.checked = "1";
   document.getElementById('fld4').innerHTML = "S60 version : " + s60_version;
	if(pagetype != 'bug_report')
   {
      form_id.h_total_ram.value = ram_info;
      form_id.h_total_ram.checked = "1";
      document.getElementById('fld5').innerHTML = "Free RAM : " + ram_info + " Kb";
      
      form_id.h_uptime.value = uptime;
      form_id.h_uptime.checked = "1";
      document.getElementById('fld6').innerHTML = "Device uptime : " + uptime + " seconds";
      
      form_id.h_country_code.value = country_dialing_code;
      form_id.h_country_code.checked = "1";
      document.getElementById('fld7').innerHTML = "Country dialing code : " + country_dialing_code;
	}

	form_id.h_imei_hash.value = imei_hash;
  	form_id.h_imei_hash.checked = "1";
   document.getElementById('fld8').innerHTML = "IMEI hash : " + imei_hash;
}

function validate_presence_details()
{
	alert('Data incomplete!!');
	return false;
}

function validate_bug_details()
{
	alert('Data incomplete!!');
	return false;
}

function validate_idea_details()
{
	alert('Data incomplete!!');
	return false;
}

function validate_ratings_details()
{
	alert('Data incomplete!!');
	return false;
}

function load_usage_data()
{
	document.getElementById('scrshot_count').innerHTML = " " + scrshot_count + " ";
}
 
