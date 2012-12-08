		
const char *CStreamChar(ostringstream SQLquery);
uint mysqlADD(uint id,uint packetype,uint subtype,uint tEvenType_gal,uint tEvenType_value,int value);
uint mysqlEXIST(uint id,uint packetype,uint subtype,uint tEvenType_gal);
uint mysqlCREATE(uint id,uint packetype,uint subtype,uint tEvenType_gal);
uint mysqlINSERT(uint mysqlcapteur,uint tEvenType_value,int value);
uint mysqlINSERTRAW(tRBUF* buffer);

const char *CStreamChar(ostringstream *SQLquery) {
	string out = SQLquery->str();
	return out.c_str();	
}

uint mysqlADD(uint id,uint packetype,uint subtype,uint tEvenType_gal,uint tEvenType_value,int value){
	uint mysqlcapteur = mysqlEXIST(id,packetype,subtype,tEvenType_gal);
	if (!mysqlcapteur) return 0;
	return mysqlINSERT(mysqlcapteur,tEvenType_value,value);
}

uint mysqlEXIST(uint id,uint packetype,uint subtype,uint tEvenType_gal) {
	ostringstream SQLquery;
	SQLquery << "SELECT `idcapteur` FROM `domo.capteur` WHERE (`id`=" << id << " AND `type`=" << packetype << " AND `subtype`=" << subtype << ")";
	if (mysql_query(MYSQLconn, CStreamChar(&SQLquery))) {
		fprintf(stderr, "%s\n", mysql_error(MYSQLconn));
		return 0;
	}
	MYSQL_RES *res=mysql_store_result(MYSQLconn);
	if (!res) {
		mysql_free_result(res);
		return mysqlCREATE(id,packetype,subtype,tEvenType_gal);	
	}else if (mysql_num_rows(res)==0) {
		mysql_free_result(res);
		return mysqlCREATE(id,packetype,subtype,tEvenType_gal);
	} else if (mysql_num_rows(res)==1) { 
		MYSQL_ROW row = mysql_fetch_row(res);
		mysql_free_result(res);
		return atoi(row[0]);
	} else {
		fprintf(stderr, "%s\n", "Plusieurs résultats pour le meme capteur");
		mysql_free_result(res);
		return 0;
	}
}
uint mysqlCREATE(uint id,uint packetype,uint subtype,uint tEvenType_gal){
	ostringstream SQLquery;
	SQLquery << "INSERT INTO `domo.capteur` (`id`,`type`,`subtype`,`eventype`) VALUES ("<< id <<","<< packetype <<","<< subtype <<","<< tEvenType_gal <<")";
	if (mysql_query(MYSQLconn, CStreamChar(&SQLquery))) {
		fprintf(stderr, "%s\n", mysql_error(MYSQLconn));
		return 0;
	}
	uint out = mysql_insert_id(MYSQLconn);
	ostringstream SQLquery2;
	SQLquery2  << "CREATE TABLE `domo.capteur." << setfill('0') << setw(4) << out <<"` (`ideventype` int(11) unsigned DEFAULT '0',`datetime` timestamp NULL DEFAULT CURRENT_TIMESTAMP,`valeur` int(11) DEFAULT '0') ENGINE=MyISAM DEFAULT CHARSET=latin1";
	if (mysql_query(MYSQLconn, CStreamChar(&SQLquery2))) {
		fprintf(stderr, "%s\n", mysql_error(MYSQLconn));
		return 0;
	}
	return out;
}
uint mysqlINSERT(uint mysqlcapteur,uint tEvenType_value,int value) {
	ostringstream SQLquery;
	if (tEvenType_value==tEvenTypeBattery) {
		SQLquery << "UPDATE `domo.capteur` SET `batt`=" << value << " WHERE `idcapteur`='"<< mysqlcapteur <<"'";
		if (mysql_query(MYSQLconn, CStreamChar(&SQLquery))) {
			fprintf(stderr, "%s\n", mysql_error(MYSQLconn));
			return 0;
		}	
	} else if (tEvenType_value==tEvenTypeSignal) {
		SQLquery << "UPDATE `domo.capteur` SET `rssi`=" << value << " WHERE `idcapteur`='"<< mysqlcapteur <<"'";
		if (mysql_query(MYSQLconn, CStreamChar(&SQLquery))) {
			fprintf(stderr, "%s\n", mysql_error(MYSQLconn));
			return 0;
		}	
	} else {
		SQLquery << "INSERT INTO `domo.capteur." << setfill('0') << setw(4) << mysqlcapteur <<"` (`ideventype`,`valeur`) VALUES ("<< tEvenType_value <<","<< value <<")";
		if (mysql_query(MYSQLconn, CStreamChar(&SQLquery))) {
			fprintf(stderr, "%s\n", mysql_error(MYSQLconn));
			return 0;
		}
	}
	return 1;
}
uint mysqlINSERTRAW(tRBUF* buffer) {
	BYTE *pbuffer;
	pbuffer=reinterpret_cast<BYTE*>(buffer);
	ostringstream SQLquery;
	SQLquery <<  "INSERT INTO `domo.capteur.unde` (`packetlength`,`packettype`,`subtype`,`seqnbr`";
	for (int i=4;i<=(*pbuffer);i++) SQLquery <<  ",`msg" << (i-3) << "`";
	SQLquery <<  ") VALUES (" << (uint)(*pbuffer);
	for (int i=1;i<=(*pbuffer);i++) SQLquery <<  "," << (uint)(*(pbuffer+i));
	SQLquery <<  ")";
	if (mysql_query(MYSQLconn, CStreamChar(&SQLquery))) {
		fprintf(stderr, "%s\n", mysql_error(MYSQLconn));
		return 0;
	}		
	return 1;
}