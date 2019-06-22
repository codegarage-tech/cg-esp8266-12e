<?php
	
	header("Access-Control-Allow-Origin: *");
	header("Content-Type: application/json; charset=UTF-8");
	
	
	//Creating Array for JSON response
	$response = array();
	
	// Check if we got the field from the user
	if (isset($_GET['wifi_status']) && isset($_GET['distance'])) {
		
		$wifi_status = $_GET['wifi_status'];
		$distance = $_GET['distance'];
		
		// Include data base connect class
		$filepath = realpath (dirname(__FILE__));
		require_once($filepath."/db_connect.php");
		
		// Connecting to database 
		$db = new DB_CONNECT();	
		
		// Update data
		// Fire SQL query to update LED status data by id
		//   $updateResult = mysql_query("UPDATE led SET status= '$status', date_time= '$date_time'  WHERE id = '$id'");
		$updateResult = mysql_query("
		UPDATE led
		SET status = CASE id 
		WHEN 4 THEN '$wifi_status' 
		WHEN 13 THEN '$distance' 
		ELSE status
		END
		WHERE id IN(4, 13)
		");
		
		// Check for succesfull execution of query and no results found
		if ($updateResult) {
			// Fire SQL query to get all data from led
			$result = mysql_query("SELECT *FROM led") or die(mysql_error());
			
			// Check for succesfull execution of query and no results found
			if (mysql_num_rows($result) > 0) {
				
				// Storing the returned array in response
				$response["led"] = array();
				
				// While loop to store all the returned response in variable
				while ($row = mysql_fetch_array($result)) {
					// temperoary user array
					$led = array();
					$led["id"] = $row["id"];
					$led["status"] = $row["status"];
					$led["date_time"] = $row["date_time"];
					
					// Push all the items 
					array_push($response["led"], $led);
				}
				// On success
				$response["success"] = 1;
				
				// Show JSON response
				echo json_encode($response);
			}else {
				// If no data is found
				$response["success"] = 0;
				$response["message"] = "No data on LED found";
				
				// Show JSON response
				echo json_encode($response);
			}   		
		} else {
			// Failed to update data
			$response["success"] = 0;
			$response["message"] = "Failed to update data";
			
			// Show JSON response
			echo json_encode($response);
		}
	} else {
		// If required parameter is missing
		$response["success"] = 0;
		$response["message"] = "Parameter(s) are missing. Please check the request";
		
		// Show JSON response
		echo json_encode($response);
	}
?>
