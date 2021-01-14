---
marp: true
theme: itp

week: 5
category: lectures
title: Data Visualization and Dashboard with Initial State (Single Values)
---

<!-- headingDivider: 2 -->

# Data Visualization and Dashboards with Initial State 

![bg opacity:.75](lecture_data_dashboards_initial_state_single_values.assets/Infruid's_Self-Service_BI_Tool_Dashboard.jpg)

## What do we do with all this data?

* A single IoT device can generate significant of sensor data
* A fleet of thousands of devices compounds this
* A **dashboard** can help provide visual feedback and control over our IoT devices and data

## Example Environmental Dashboard

<img src="lecture_data_dashboards_initial_state_single_values.assets/image-20200724004339825.png" alt="image-20200724004339825" style="width:400px;" /><img src="lecture_data_dashboards_initial_state_single_values.assets/image-20200724005050658.png" alt="image-20200724005050658" style="width:400px;" />

## Uses of Dashboards

* View real-time sensor data
* Compare data to historical trends
* Monitor IoT devices for errors or network issues
* Map devices GPS location
* Remote control (execute functions) on devices

## Dashboard Platforms

* [Google Cloud IoT](https://cloud.google.com/solutions/iot) (many IoT services)
* [Microsoft Azure IoT](https://azure.microsoft.com/en-us/overview/iot/) (many IoT services)
*  [Amazon Web Services](https://aws.amazon.com/iot/) (many IoT services)
* [Losant](https://www.losant.com/) (many IoT services)
* [Ubidots](https://ubidots.com/)
* [Tinamous](https://www.tinamous.com/)
* [Initial State](https://www.initialstate.com/)

## Initial State ([https://initialstate.com](https://initialstate.com))

* Initial State is a data visualization service for IoT
* Send data from Argon to be stored at Initial State 
* Create complex graphs and dashboard with the values from your Argon
* Generous free tier!
* Initial State can also send SMS and email alerts based on the data (though this requires a paid plan)

## Configuring Argon and Initial State 

* The following examples are provided as a quick reference
* More detailed steps are described in the lecture 

## Steps to Connect Argon and Initial State 

1. Create account at  [https://initialstate.com](https://initialstate.com) 
2. Create **stream bucket** at [https://initialstate.com](https://initialstate.com)  
3. Create an integration -> webhook on [Particle console](https://console.particle.io/integrations) 
4. Write firmware sketch and flash Argon

## Aside: Access Key (or API key)

* Many webservices require you to register and then they provide you with an API key (ex: `zaCELgL. 0imfnc8mVLWwsAawjYr4Rx-Af50DDqtlx`)
* This randomly-generated string serves as the login and password (credentials) to your account
* Purposes
  * Account security (no one but you can read / write data)
  * Prevent abuse / hacking
  * Throttling / account limits (enforce limits on read/write frequency)

## Example 1: Send Single Data Point from Argon to Initial State

**Initial State Settings**

<img src="lecture_data_dashboards_initial_state_single_values.assets/image-20200703180348874.png" alt="image-20200703180348874" style="width:500px;" />

## Example 1: Send Single Data Point from Argon to Initial State

**Particle integration settings**

<img src="lecture_data_dashboards_initial_state_single_values.assets/image-20200703160032339.png" alt="image-20200703160032339" style="width:600px;" />

## Example 1: Send Single Data Point from Argon to Initial State

**Particle integration settings**

JSON Data: `{{{PARTICLE_EVENT_VALUE}}}`

URL: `https://groker.init.st/api/events`

<img src="lecture_data_dashboards_initial_state_single_values.assets/image-20200703180944077.png" alt="image-20200703180944077" style="width:500px;" />

## Example 1: Send Single Data Point from Argon to Initial State

**Particle integration settings**

<img src="lecture_data_dashboards_initial_state_single_values.assets/SNAGHTML22743719.PNG" alt="img" style="width:500px;" />



## Example 1: Send Single Data Point from Argon to Initial State

**Particle integration setting: HTTP Headers**

`Accept-Version`	>	`~0`      ***note: this is a tilde ~, NOT a hyphen -***

`X-IS-AccessKey`	>	*<<USE YOUR OWN API KEY>>>*

`X-IS-BucketKey`	>	*<<USE YOUR OWN BUCKET KEY>>>*




## Example 1: Send Single Data Point from Argon to Initial State

* Initial State expects data to be entered in JSON format

```json
{"key": "temp", "value": 32}
```

*Where `temp` is the your chosen data label, and `32` is the current value*

* Each time we send data to Initial State, we sent one single data point

*We will cover JSON in-depth next week. For now, we will use this format with JSON format with `{}` to send data*

## Recall: Escape Characters

* Initial State value contains quotation marks `"` in the value

```json
{"key": "temp", "value": 32}
```

* Since `"` marks the beginning and end of `String`, we need to tell the Argon specifically when we want to include `"` in the value
* We use the **escape character ** `\"` to tell the Argon to ignore the `"` and consider it as part of the `String`
* Thus we have our variable would become

```json
String msg = "{\"key\": \"temp\", \"value\": 32}";
```



## Example 1: Send Single Data Point from Argon to Initial State

**Argon code**

```c++
double temp;

void loop() {
  temp = 89;		//just example; temp should come from sensor
    
  String data = "{ \"key\":\"temperature\","   
                "\"value\":" + String(temp)+"}";
  // Trigger the integration
  Particle.publish("inital_state_json", String(temp), PRIVATE);
  delay(60000);	//use delay or millis to avoid publishing too frequently
}
```



## Lab - Send Photoresistor Value to Initial State

* Connect photoresistor to Argon
* Read photoresistor value and send to InitialState

## Lab - Send Photoresistor Value to Initial State

<img src="lecture_data_dashboards_initial_state_single_values.assets/photoresistor_and_fixed_resistor_bb.png" style="width:500px;" />



## Credits

* [Growthlakes](https://commons.wikimedia.org/wiki/File:Infruid%27s_Self-Service_BI_Tool_Dashboard.jpg)  [CC BY-SA](https://creativecommons.org/licenses/by-sa/4.0)
* Image created with [Fritzing](https://fritzing.org)