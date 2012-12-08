#include "RFXtrx.h"

#define tEvenTypeUnknonw		1
#define tEvenTypeSignal			2
#define tEvenTypeTemperature	4
#define tEvenTypeHumidity		8
#define tEvenTypeSpeed			16
#define tEvenTypeMeanSpeed		32
#define tEvenTypeDirection		64
#define tEvenTypePower			128
#define tEvenTypeState			256
#define tEvenTypeBattery		512

#include "RFXMySQL.cpp"

uint analyse_tRBUF(tRBUF *message);
typedef uint (*fType)              (tRBUF *       );
		uint fTypeInterfaceMessage (tRBUF *message);
		uint fTypeUndecoded        (tRBUF *message);
		uint fTypeLighting1        (tRBUF *message);
		uint fTypeLighting2        (tRBUF *message);
		uint fTypeLighting3        (tRBUF *message);
		uint fTypeLighting4        (tRBUF *message);
		uint fTypeLighting5        (tRBUF *message);
		uint fTypeLighting6        (tRBUF *message);
		uint fTypeCurtain          (tRBUF *message);
		uint fTypeBlinds           (tRBUF *message);
		uint fTypeSecurity1        (tRBUF *message);
		uint fTypeCamera           (tRBUF *message);
		uint fTypeRemote           (tRBUF *message);
		uint fTypeThermostat1      (tRBUF *message);
		uint fTypeThermostat2      (tRBUF *message);
		uint fTypeThermostat3      (tRBUF *message);
		uint fTypeTEMP             (tRBUF *message);
		uint fTypeHUM              (tRBUF *message);
		uint fTypeTEMP_HUM         (tRBUF *message);
		uint fTypeBARO             (tRBUF *message);
		uint fTypeTEMP_HUM_BARO    (tRBUF *message);
		uint fTypeRAIN             (tRBUF *message);
		uint fTypeWIND             (tRBUF *message);
		uint fTypeUV               (tRBUF *message);
		uint fTypeDT               (tRBUF *message);
		uint fTypeCURRENT          (tRBUF *message);
		uint fTypeENERGY           (tRBUF *message);
		uint fTypeGAS              (tRBUF *message);
		uint fTypeWATER            (tRBUF *message);
		uint fTypeWEIGHT           (tRBUF *message);
		uint fTypeRFXSensor        (tRBUF *message);
		uint fTypeRFXMeter         (tRBUF *message);
		uint fTypeFS20             (tRBUF *message);
		uint fTypeDefault          (tRBUF *message);

uint analyse_tRBUF(tRBUF *message) {
	fType pfType;
	switch (message->UNDECODED.packettype) {
		case pTypeInterfaceMessage : pfType  = fTypeInterfaceMessage ; break;
		case pTypeUndecoded        : pfType  = fTypeUndecoded        ; break;
		case pTypeLighting1        : pfType  = fTypeLighting1        ; break;
		case pTypeLighting2        : pfType  = fTypeLighting2        ; break;
		case pTypeLighting3        : pfType  = fTypeLighting3        ; break;
		case pTypeLighting4        : pfType  = fTypeLighting4        ; break;
		case pTypeLighting5        : pfType  = fTypeLighting5        ; break;
		case pTypeLighting6        : pfType  = fTypeLighting6        ; break;
		case pTypeCurtain          : pfType  = fTypeCurtain          ; break;
		case pTypeBlinds           : pfType  = fTypeBlinds           ; break;
		case pTypeSecurity1        : pfType  = fTypeSecurity1        ; break;
		case pTypeCamera           : pfType  = fTypeCamera           ; break;
		case pTypeRemote           : pfType  = fTypeRemote           ; break;
		case pTypeThermostat1      : pfType  = fTypeThermostat1      ; break;
		case pTypeThermostat2      : pfType  = fTypeThermostat2      ; break;
		case pTypeThermostat3      : pfType  = fTypeThermostat3      ; break;
		case pTypeTEMP             : pfType  = fTypeTEMP             ; break;
		case pTypeHUM              : pfType  = fTypeHUM              ; break;
		case pTypeTEMP_HUM         : pfType  = fTypeTEMP_HUM         ; break;
		case pTypeBARO             : pfType  = fTypeBARO             ; break;
		case pTypeTEMP_HUM_BARO	   : pfType  = fTypeTEMP_HUM_BARO    ; break;
		case pTypeRAIN         	   : pfType  = fTypeRAIN             ; break;
		case pTypeWIND         	   : pfType  = fTypeWIND             ; break;
		case pTypeUV           	   : pfType  = fTypeUV               ; break;
		case pTypeDT           	   : pfType  = fTypeDT               ; break;
		case pTypeCURRENT      	   : pfType  = fTypeCURRENT          ; break;
		case pTypeENERGY       	   : pfType  = fTypeENERGY           ; break;
		case pTypeGAS          	   : pfType  = fTypeGAS              ; break;
		case pTypeWATER        	   : pfType  = fTypeWATER            ; break;
		case pTypeWEIGHT       	   : pfType  = fTypeWEIGHT           ; break;
		case pTypeRFXSensor    	   : pfType  = fTypeRFXSensor        ; break;
		case pTypeRFXMeter     	   : pfType  = fTypeRFXMeter         ; break;
		case pTypeFS20         	   : pfType  = fTypeFS20             ; break;
		default              	   : pfType  = fTypeDefault          ; break;
	}
	return pfType(message);
}

uint fTypeInterfaceMessage (tRBUF *message){
	uint packetype = message->IRESPONSE.packettype;
	uint subtype = message->IRESPONSE.subtype;
	uint cmnd = message->IRESPONSE.cmnd;	
	uint msg1 = message->IRESPONSE.msg1;	
	uint msg2 = message->IRESPONSE.msg2;	
	uint msg3 = message->IRESPONSE.msg3;	
	uint msg4 = message->IRESPONSE.msg4;	
	uint msg5 = message->IRESPONSE.msg5;	
	uint msg6 = message->IRESPONSE.msg6;	
	uint msg7 = message->IRESPONSE.msg7;	
	uint msg8 = message->IRESPONSE.msg8;	
	uint msg9 = message->IRESPONSE.msg9;	

	printf("RFXCOM:Sub:%.2d | CMD:%d | MSG:%02hhX.%02hhX.%02hhX.%02hhX.%02hhX.%02hhX.%02hhX.%02hhX.%02hhX\n", subtype, cmnd, msg1, msg2, msg3, msg4, msg5, msg6, msg7, msg8, msg9);
	return 1;
}
uint fTypeUndecoded        (tRBUF *message){
	BYTE *pbuffer;
	pbuffer=reinterpret_cast<BYTE*>(message);
	printf("RAWRCV:L:%.2d | T:%02hhX | ST:%02hhX | SEQ=%02hhX | M:", *(pbuffer), *(pbuffer+1), *(pbuffer+2), *(pbuffer+3));
	for (int i=4;i<=(*pbuffer);i++) printf("%02hhX.",(*(pbuffer+i)));
	printf("\n");
	mysqlINSERTRAW(message);
	return 1;
}
uint fTypeLighting1        (tRBUF *message){return 0;}
uint fTypeLighting2        (tRBUF *message){
	uint packetype = message->LIGHTING2.packettype;
	uint subtype = message->LIGHTING2.subtype;
	uint id = 0x1000000*message->LIGHTING2.id1 + 0x10000*message->LIGHTING2.id2 + 0x100*message->LIGHTING2.id3 + message->LIGHTING2.id4;
	uint unitcode = message->LIGHTING2.unitcode;
	uint cmnd = message->LIGHTING2.cmnd;
	uint level = message->LIGHTING2.cmnd*6;
	uint rssi = message->LIGHTING2.rssi*6;

	printf("LIGHT2:Sub:%.2d | id:%d | UC:%d C:%d L:%d \n",subtype,id,unitcode,cmnd,level);
	uint tEvenType = tEvenTypeState | tEvenTypeSignal;
	mysqlADD(id,packetype,subtype,tEvenType,tEvenTypeState,cmnd);		
	mysqlADD(id,packetype,subtype,tEvenType,tEvenTypeSignal,rssi);		
	return 1;
}
uint fTypeLighting3        (tRBUF *message){return 0;}
uint fTypeLighting4        (tRBUF *message){return 0;}
uint fTypeLighting5        (tRBUF *message){return 0;}
uint fTypeLighting6        (tRBUF *message){return 0;}
uint fTypeCurtain          (tRBUF *message){return 0;}
uint fTypeBlinds           (tRBUF *message){return 0;}
uint fTypeSecurity1        (tRBUF *message){return 0;}
uint fTypeCamera           (tRBUF *message){return 0;}
uint fTypeRemote           (tRBUF *message){return 0;}
uint fTypeThermostat1      (tRBUF *message){return 0;}
uint fTypeThermostat2      (tRBUF *message){return 0;}
uint fTypeThermostat3      (tRBUF *message){return 0;}
uint fTypeTEMP             (tRBUF *message){
	uint packetype = message->TEMP.packettype;
	uint subtype = message->TEMP.subtype;
	uint id = 0x100*message->TEMP.id1 + message->TEMP.id2;
	int temperature = message->TEMP.temperaturel + 0x100*message->TEMP.temperatureh;		
	temperature = message->TEMP.tempsign ? temperature*-1 : temperature;
	uint battery_level = (message->TEMP.battery_level+1)*10;
	uint rssi = message->TEMP.rssi*6;

	printf("TEMP  :Sub:%.2d | id:%d | T:%.1f°C B:%d%%\n",subtype,id,temperature/10.,battery_level);
	uint tEvenType = tEvenTypeTemperature | tEvenTypeBattery | tEvenTypeSignal;
	mysqlADD(id,packetype,subtype,tEvenType,tEvenTypeTemperature,temperature);	
	mysqlADD(id,packetype,subtype,tEvenType,tEvenTypeBattery,battery_level);	
	mysqlADD(id,packetype,subtype,tEvenType,tEvenTypeSignal,rssi);		
	return 1;
}
uint fTypeHUM              (tRBUF *message){return 0;}
uint fTypeTEMP_HUM         (tRBUF *message){
	uint packetype = message->TEMP_HUM.packettype;
	uint subtype = message->TEMP_HUM.subtype;
	uint id = 0x100*message->TEMP_HUM.id1 + message->TEMP_HUM.id2;
	int temperature = message->TEMP_HUM.temperaturel + 0x100*message->TEMP_HUM.temperatureh;		
	temperature = message->TEMP_HUM.tempsign ? temperature*-1 : temperature;
	uint humidity = message->TEMP_HUM.humidity;
	uint humidity_status = message->TEMP_HUM.humidity_status;
	uint battery_level = (message->TEMP_HUM.battery_level+1)*10;
	uint rssi = message->TEMP_HUM.rssi*6;

	printf("TEMPHU:Sub:%.2d | id:%d | T:%.1f°C H:%d%% HS:%d B:%d%%\n",subtype,id,temperature/10.,humidity,humidity_status,battery_level);
	uint tEvenType = tEvenTypeTemperature | tEvenTypeHumidity | tEvenTypeBattery | tEvenTypeSignal;
	mysqlADD(id,packetype,subtype,tEvenType,tEvenTypeTemperature,temperature);	
	mysqlADD(id,packetype,subtype,tEvenType,tEvenTypeHumidity,humidity);	
	mysqlADD(id,packetype,subtype,tEvenType,tEvenTypeBattery,battery_level);
	mysqlADD(id,packetype,subtype,tEvenType,tEvenTypeSignal,rssi);		
	return 1;
}
uint fTypeBARO             (tRBUF *message){return 0;}
uint fTypeTEMP_HUM_BARO    (tRBUF *message){return 0;}
uint fTypeRAIN             (tRBUF *message){return 0;}
uint fTypeWIND             (tRBUF *message){
	uint packetype = message->WIND.packettype;
	uint subtype = message->WIND.subtype;
	uint id = 0x100*message->WIND.id1 + message->WIND.id2;
	uint direction = message->WIND.directionl + 0x100*message->WIND.directionh;		
	int av_speed = message->WIND.av_speedl + 0x100*message->WIND.av_speedh;		
	int gust = message->WIND.gustl + 0x100*message->WIND.gusth;		
	int temperature = message->WIND.temperaturel + 0x100*message->WIND.temperatureh;		
	temperature = message->WIND.tempsign ? temperature*-1 : temperature;
	int chill = message->WIND.chilll + 0x100*message->WIND.chillh;		
	chill = message->WIND.chillsign ? chill*-1 : chill;
	if (subtype!=0x04) {temperature=0;chill=0;}
	uint battery_level = (message->WIND.battery_level+1)*10;
	uint rssi = message->WIND.rssi*6;
	
	printf("ANEMOM:Sub:%.2d | id:%d | D:%d° SA:%.1fm/s S:%.1fm/s B:%d%%\n",subtype,id,direction,av_speed/10.,gust/10.,battery_level);
	uint tEvenType = tEvenTypeDirection | tEvenTypeMeanSpeed | tEvenTypeSpeed | tEvenTypeBattery | tEvenTypeSignal;
	mysqlADD(id,packetype,subtype,tEvenType,tEvenTypeDirection,direction);
	mysqlADD(id,packetype,subtype,tEvenType,tEvenTypeMeanSpeed,av_speed);
	mysqlADD(id,packetype,subtype,tEvenType,tEvenTypeSpeed,gust);
	mysqlADD(id,packetype,subtype,tEvenType,tEvenTypeBattery,battery_level);
	mysqlADD(id,packetype,subtype,tEvenType,tEvenTypeSignal,rssi);		
	return 1;
}
uint fTypeUV               (tRBUF *message){return 0;}
uint fTypeDT               (tRBUF *message){return 0;}
uint fTypeCURRENT          (tRBUF *message){return 0;}
uint fTypeENERGY           (tRBUF *message){return 0;}
uint fTypeGAS              (tRBUF *message){return 0;}
uint fTypeWATER            (tRBUF *message){return 0;}
uint fTypeWEIGHT           (tRBUF *message){return 0;}
uint fTypeRFXSensor        (tRBUF *message){return 0;}
uint fTypeRFXMeter         (tRBUF *message){
	uint packetype = message->RFXMETER.packettype;
	uint subtype = message->RFXMETER.subtype;
	uint id = 0x100*message->RFXMETER.id1 + message->RFXMETER.id2;
	uint count = 0x1000000*message->RFXMETER.count1 + 0x10000*message->RFXMETER.count2 + 0x100*message->RFXMETER.count3 + message->RFXMETER.count4;
	uint rssi = message->RFXMETER.rssi*6;		

	printf("RFXMET:Sub:%.2d | id:%d | C:%d\n",subtype,id,count);
	uint tEvenType = tEvenTypePower | tEvenTypeSignal;
	mysqlADD(id,packetype,subtype,tEvenType,tEvenTypePower,count);
	mysqlADD(id,packetype,subtype,tEvenType,tEvenTypeSignal,rssi);		
	return 1;
}
uint fTypeFS20             (tRBUF *message){return 0;}
uint fTypeDefault          (tRBUF *message){return 0;}