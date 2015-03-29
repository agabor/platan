/*     
 * 	  Copyright 2015 Jens Schyma jeschyma@gmail.com
 *		
 *	  This File is a Part of the source of Open-Fin-TS-JS-Client.
 *
 *    This program is free software: you can redistribute it and/or  modify
 *    it under the terms of the GNU Affero General Public License, version 3,
 *    as published by the Free Software Foundation.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU Affero General Public License for more details.
 *
 *    You should have received a copy of the GNU Affero General Public License
 *    along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *	  Please contact Jens Schyma if you are interested in a commercial license.
 *	
 */


var logger = function(area){
    this.area = area;
    this.debug = function(json,str){print(str)}
    this.error = function(e,json,str){print(str)}
    this.trace = function(json,str){print(str)}
    this.child = function(area){
        return new logger(area);
    }
}
var ConsoleT = function(){
    this.log = function(msg){print(msg)}
}
var Console = new ConsoleT();

var ParseError = function (area, txt, pos) {
    this.t = txt;
    this.toString = function(){
        return this.t;
    };
};

var Parser = function (in_txt) {
    var me = this;
    me.data 		= in_txt;
    me.cur_pos 		= 0;
    me.marker 		= {};
    me.clearMarker 	= function () {
        me.marker = {};
    };
    me.setMarker = function (mark, pos) {
        me.marker[mark] = pos;
    };
    me.setMarkerWithCurrentPos = function (mark) {
        me.setMarker(mark, me.cur_pos);
    };
    me.setPosBackToMarker = function (mark) {
        me.cur_pos = me.marker[mark];
    };
    me.getCurrentPos = function () {
        return this.cur_pos;
    };
    me.setCurrentPos = function (pos) {
        me.cur_pos = pos;
    };
    me.getCurrentChar = function () {
        return me.data[me.cur_pos];
    }
    me.hasNext = function () {
        if (me.cur_pos < me.data.length) {
            return true;
        } else {
            return false;
        }
    };
    me.nextPos = function () {
        if (me.cur_pos < me.data.length) {
            me.cur_pos++;
            return true;
        } else {
            return false;
        }
    };
    me.getTextFromMarkerToCurrentPos = function (mark) {
        return me.getTextFromPostoPos(me.marker[mark], me.cur_pos);
    };
    me.getTextFromPostoPos = function (pos1, pos2) {
        return me.data.substr(pos1, pos2 - pos1);
    };
    me.findNextValidChar = function (valid_chars) {
        var i = 0;
        for (i = me.cur_pos; i < me.data.length; i++) {
            if (valid_chars.indexOf(me.data[i]) != -1) {
                return i;
            }
        }
        return -1;
    };
    me.gotoNextValidChar = function (valid_chars) {
        var pos = me.findNextValidChar(valid_chars);
        if (pos == -1) {
            me.cur_pos = me.data.length;
            return false;
        } else {
            me.cur_pos = pos;
            return true;
        }
    };
    me.gotoNextValidCharButIgnoreWith = function (valid_chars, demask) {
        while (true) {
            var pos = me.findNextValidChar(valid_chars);
            if (pos == -1) {
                me.cur_pos = me.data.length;
                return false;
            } else if (pos == 0) {
                me.cur_pos = pos;
                return true;
            } else if (demask.indexOf(me.data[pos - 1]) != -1) {
                if ((pos + 1) < me.data.length) {
                    me.cur_pos = pos + 1;
                    // retry
                } else {
                    me.cur_pos = pos;
                    return false;
                }
            } else {
                me.cur_pos = pos;
                return true;
            }
        }
    };
};

var Konto = function(){
    var me_konto = this;
    me_konto.iban = "";
    me_konto.konto_nr ="";
    me_konto.unter_konto_merkm = null;
    me_konto.ctry_code = "";
    me_konto.blz = "";
    me_konto.kunden_id = "";
    me_konto.kontoart = "";
    me_konto.currency = "";
    me_konto.kunde1_name = "";
    me_konto.product_name = "";
};
var NULL = new function(){
    this.id = 1234;
};

var ByteVal = function(ddd){
    this.data =  ddd;
};

var Helper = new function () {
    this.checkMsgsWithBelongToForId = function(msg,bez,id){
        var array = msg.selectSegByNameAndBelongTo("HIRMS",bez);
        if(array.length>0){
            for(var i=0;i!=array.length;i++){
                for(var a=0;a!=array[i].store.data.length;a++){
                    var d= array[i].store.data[a];
                    if(d.getEl(1)==id){
                        return d;
                    }
                }
            }
            return null;
        }else{
            return null;
        }

    }
    this.getNrWithLeadingNulls = function (nr, len) {
        var stxt = nr + "";
        var ltxt = "";
        var neu = len - stxt.length;
        for (var i = 0; i != neu; i++) {
            ltxt += "0";
        }
        ltxt += stxt;
        return ltxt;
    };
    this.newSegFromArrayWithBez = function (name, vers,bez, ar) {
        var seg = this.newSegFromArray(name,vers,ar);
        seg.bez = bez;
        return seg;
    }
    this.newSegFromArray = function (name, vers, ar) {
        var seg = new Segment();
        seg.init(name, 0, vers, 0);
        for (var i = 0; i != ar.length; i++) {
            if (ar[i] instanceof Array) {
                var neu = new DatenElementGruppe();
                for (var j = 0; j != ar[i].length; j++) {
                    if(ar[i][j] instanceof ByteVal){
                        neu.addDEbin(ar[i][j].data);
                    }else{
                        neu.addDE(ar[i][j]);
                    }
                }
                seg.store.addDEG(neu);
            } else if(ar[i] instanceof ByteVal){
                seg.store.addDEbin(ar[i].data);
            }else {
                // normales datenelement
                seg.store.addDE(ar[i]);
            }
        }
        return seg;
    };
    this.convertIntoArray = function(de_or_deg){
        if(de_or_deg instanceof DatenElementGruppe){
            var r=[];
            for(var i=0;i!=de_or_deg.data.length;i++){
                r.push(de_or_deg.data[i]);
            }
            return r;
        }else{
            return [de_or_deg];
        }
    };
    this.convertDateToDFormat = function(date){
        var yyyy = date.getFullYear()+"";
        var mm   = ((date.getMonth()+1)<=9)?("0"+(date.getMonth()+1)):((date.getMonth()+1)+"");
        var dd   = (date.getDate()<=9)?("0"+date.getDate()):(date.getDate()+"");
        return yyyy+mm+dd;
    };
    this.convertDateToTFormat = function(date){
        var hh = ((date.getHours()<=9)?"0":"")+date.getHours();
        var mm = ((date.getMinutes()<=9)?"0":"")+date.getMinutes();
        var ss = ((date.getSeconds()<=9)?"0":"")+date.getSeconds();
        return hh+mm+ss;
    };
    this.convertFromToJSText = function(ftxt){
        var jstxt = "";
        var re = /\?([^\?])/g;
        jstxt = ftxt.replace(re,"$1");
        return jstxt;
    };
    this.convertJSTextTo = function(jstxt){
        var ftxt = "";
        var re = /([:\+\?'\@])/g;
        ftxt = jstxt.replace(re,"?$&");
        return ftxt;
    };
    this.Byte = function(data){
        return new ByteVal(data);
    };
    this.getSaldo = function(seg,nr,hbci_3_vers){
        if(seg){
            try{
                var base = seg.getEl(nr);
                var result = {
                    'soll_haben':		null,
                    'buchungsdatum' : 	null,
                    'currency':			null,
                    'value':			null
                };
                result.soll_haben = base.getEl(1)=="C"?"H":"S";
                result.currency   = hbci_3_vers?"EUR":base.getEl(3);
                result.value	  = parseFloat(base.getEl(2).replace(',', '.'));
                result.buchungsdatum = this.getJSDateFromSeg(base,hbci_3_vers?3:4,hbci_3_vers?4:5);
                return result;
            }catch(ee){
                return null;
            }
        }else{
            return null;
        }
    };
    this.getBetrag = function(seg,nr){
        if(seg){
            try{
                var base = seg.getEl(nr);
                var result = {
                    'currency':			null,
                    'value':			null
                };
                result.currency   = base.getEl(2);
                result.value	  = parseFloat(base.getEl(1).replace(',', '.'));
                return result;
            }catch(ee){
                return null;
            }
        }else{
            return null;
        }
    };
    this.getJSDateFromSegTSP = function(seg,nr){
        try{
            var base = seg.getEl(nr);
            return this.getJSDateFromSeg(base,1,2);
        }catch(e){
            return null;
        }
    };
    this.getJSDateFromSeg = function(seg,date_nr,time_nr){
        if(seg){
            try{
                var date = seg.getEl(date_nr);
                var time = "000000";
                try{ if(time_nr) time = seg.getEl(time_nr); }catch(eee){}
                var result = new Date();
                result.setTime(0);
                result.setYear(parseInt(date.substr(0,4),10));
                result.setMonth(parseInt(date.substr(4,2),10)-1);
                result.setDate(parseInt(date.substr(6,2),10));
                result.setHours(parseInt(time.substr(0,2),10));
                result.setMinutes(parseInt(time.substr(2,2),10));
                result.setSeconds(parseInt(time.substr(4,2),10));
                return result;
            }catch(ee){
                return null;
            }
        }else{
            return null;
        }
    };
};

var DatenElementGruppe = function () {
    var me_deg = this;
    me_deg.next_el = 0;
    me_deg.data = new Array();
    me_deg.desc = new Array();
    me_deg.addDE = function (val) {
        me_deg.data[me_deg.next_el] = val;
        me_deg.desc[me_deg.next_el] = 1;
        me_deg.next_el++;
    };
    me_deg.addDEbin = function (val) {
        me_deg.data[me_deg.next_el] = val;
        me_deg.desc[me_deg.next_el] = 3;
        me_deg.next_el++;
    };
    me_deg.addDEG = function (grup) {
        me_deg.data[me_deg.next_el] = grup;
        me_deg.desc[me_deg.next_el] = 2;
        me_deg.next_el++;
    };
    me_deg.parse = function (parser) {
        var start_pos;
        var first = false;
        while (!first || (":" == parser.getCurrentChar() && parser.hasNext())) {
            if (!first) first = true;
            else parser.nextPos();
            start_pos = parser.getCurrentPos();
            parser.setMarkerWithCurrentPos("start");
            if (parser.getCurrentChar() == "@") {
                // binary
                parser.nextPos();
                parser.setMarkerWithCurrentPos("start");
                if (!parser.gotoNextValidChar("@")) throw new ParseError("Seg", "Error binary!", start_pos);
                var len = parseInt(parser.getTextFromMarkerToCurrentPos("start"),10);
                parser.nextPos();
                parser.setMarkerWithCurrentPos("start");
                parser.setCurrentPos(parser.getCurrentPos() + len);
                if ("+:'".indexOf(parser.getCurrentChar()) == -1) throw new ParseError("Seg", "Error binary, Wrong Length!" + len, start_pos);
                me_deg.addDEbin(parser.getTextFromMarkerToCurrentPos("start"));
                parser.nextPos();
            } else if (parser.gotoNextValidCharButIgnoreWith("+:'", "?")) {
                // Normales datenelement
                me_deg.addDE(parser.getTextFromMarkerToCurrentPos("start"));
                // Datengruppe können nicht bestandteil einer datengruppe sein
            } else {
                throw new ParseError("Seg", "Unerwartetes ENDE!", start_pos);
            }
        }
    };
    me_deg.transformForSend = function () {
        var result = "";
        for (var i = 0; i != me_deg.data.length; i++) {
            if(me_deg.data[i]!=NULL){
                if (me_deg.desc[i] == 1) {
                    result += (i != 0 ? ":" : "") + me_deg.data[i]; // DE
                } else if (me_deg.desc[i] == 2) { // kommt nicht vor
                    result += (i != 0 ? ":" : "") + me_deg.data[i].transformForSend(); //DEG
                } else if (me_deg.desc[i] == 3) {
                    result += (i != 0 ? ":" : "") + "@" + me_deg.data[i].length + "@" + me_deg.data[i]; // BIN DAT
                }
            }else{
                // leer
                result += (i != 0 ? ":" : "");
            }
        }
        return result;
    };
    me_deg.getEl = function(i){
        return me_deg.data[i-1];
    };
};

var Segment = function () {


    var me_seg = this;
    me_seg.name = null;
    me_seg.nr = 0;
    me_seg.vers = 0;
    me_seg.bez = 0;
    me_seg.store = new DatenElementGruppe();

    me_seg.init = function (n, nr, ve, be) {
        me_seg.name = n;
        me_seg.nr = nr;
        me_seg.vers = ve;
        me_seg.bez = be;
    };

    me_seg.transformForSend = function () {
        var result = "";
        result += me_seg.name; // Nr. 1 Segmentkennung an ..6 M 1
        result += ":" + me_seg.nr; // Nr. 2 Segmentnummer num ..3 M 1 >=1
        result += ":" + me_seg.vers; // Nr. 3 Segmentversion GD num ..3 M 1
        if(me_seg.bez!==0) result += ":" + me_seg.bez;
        for (var i = 0; i != me_seg.store.data.length; i++) {
            if(me_seg.store.data[i]!=NULL){
                if (me_seg.store.desc[i] == 1) {
                    result += "+" + me_seg.store.data[i]; // DE
                } else if (me_seg.store.desc[i] == 2) {
                    result += "+" + me_seg.store.data[i].transformForSend(); //DEG
                } else if (me_seg.store.desc[i] == 3) {
                    result += "+@" + me_seg.store.data[i].length + "@" + me_seg.store.data[i]; // BIN DAT
                }
            }else{
                // leer
                result += "+";
            }
        }
        result += "'";
        return result;
    };

    me_seg.parse = function (parser) {
        var start_pos = parser.getCurrentPos();
        // 1. Segmentkopf
        // Nr. 1 Segmentkennung an ..6 M 1
        parser.setMarkerWithCurrentPos("start");
        if (parser.gotoNextValidChar(":")) {
            me_seg.name = parser.getTextFromMarkerToCurrentPos("start");
        } else {
            throw new ParseError("Seg", "Segmentkennung Fehlt!", start_pos);
        }
        // Nr. 2 Segmentnummer num ..3 M 1 >=1
        parser.nextPos();
        start_pos = parser.getCurrentPos();
        parser.setMarkerWithCurrentPos("start");
        if (parser.gotoNextValidChar(":")) {
            me_seg.nr = parser.getTextFromMarkerToCurrentPos("start");
        } else {
            throw new ParseError("Seg", "Segmentnummer fehlt!", start_pos);
        }
        // Nr. 3 Segmentversion GD num ..3 M 1
        parser.nextPos();
        start_pos = parser.getCurrentPos();
        parser.setMarkerWithCurrentPos("start");
        if (parser.gotoNextValidChar(":+'")) {
            me_seg.vers = parser.getTextFromMarkerToCurrentPos("start");
        } else {
            throw new ParseError("Seg", "Segmentversion fehlt!", start_pos);
        }
        // Nr. 4 Bezugssegment GD num ..3 K 1 >=1
        if (parser.getCurrentChar() == ":") {
            parser.nextPos();
            start_pos = parser.getCurrentPos();
            parser.setMarkerWithCurrentPos("start");
            if (parser.gotoNextValidChar("+")) {
                me_seg.bez = parser.getTextFromMarkerToCurrentPos("start");
            } else {
                throw new ParseError("Seg", "Unerwartetes ENDE!", start_pos);
            }
        }
        // jetzt kommen datenlemente oder datenelementgruppen
        while (parser.getCurrentChar() != "'" && parser.hasNext()) {
            parser.nextPos();
            start_pos = parser.getCurrentPos();
            parser.setMarkerWithCurrentPos("start");
            if (parser.getCurrentChar() == "@") {
                // binary
                parser.nextPos();
                parser.setMarkerWithCurrentPos("start");
                if (!parser.gotoNextValidChar("@")) throw new ParseError("Seg", "Error binary!", start_pos);
                var len = parseInt(parser.getTextFromMarkerToCurrentPos("start"),10);
                parser.nextPos();
                parser.setMarkerWithCurrentPos("start");
                parser.setCurrentPos(parser.getCurrentPos() + len);
                if ("+:'".indexOf(parser.getCurrentChar()) == -1) throw new ParseError("Seg", "Error binary, Wrong Length!" + len, start_pos);
                me_seg.store.addDEbin(parser.getTextFromMarkerToCurrentPos("start"));
            } else if (parser.gotoNextValidCharButIgnoreWith("+:'", "?")) {
                if (parser.getCurrentChar() == "+" || parser.getCurrentChar() == "'") {
                    // Normales datenelement
                    me_seg.store.addDE(parser.getTextFromMarkerToCurrentPos("start"));
                } else {
                    // Datengruppe
                    parser.setPosBackToMarker("start");
                    var neu_deg = new DatenElementGruppe();
                    neu_deg.parse(parser);
                    me_seg.store.addDEG(neu_deg);
                }
            } else {
                throw new ParseError("Seg", "Unerwartetes ENDE!", start_pos);
            }
        }
    };

    me_seg.getEl = function(nr){
        return me_seg.store.data[nr-1];
    };
};

var Nachricht = function (proto_version) {
    var me_msg = this;
    me_msg.segments = new Array();
    me_msg.segments_ctr = 0;
    me_msg.sign_it = null;
    me_msg.hnvsk   = null;
    me_msg.msg_nr  = 0;
    me_msg.proto_version = proto_version;

    me_msg.sign = function(sign_obj){// sign_obj = {'pin':pin,'tan':tan,'sys_id':0}// Tan bitte null setzen wenn nicht benötigt
        me_msg.sign_it = sign_obj;
    };

    me_msg.init = function (dialog_id, ongoing_nr,blz,kunden_id) {
        // this is called wenn es ein outgoing message ist
        me_msg.msg_nr = ongoing_nr;
        var seg = new Segment();
        seg.init("HNHBK", 1, 3, 0);
        me_msg.addSeg(seg);
        seg.store.addDE(Helper.getNrWithLeadingNulls(0, 12)); // Länge
        seg.store.addDE(me_msg.proto_version+""); // Version
        seg.store.addDE(dialog_id); //Dialog-ID, bei 0 beginnend wird von KI bekannt gegeben
        seg.store.addDE(me_msg.msg_nr); //Nachrichten-Nr. streng monoton von 1 ab steigen
        if(me_msg.sign_it){// NUr für das Pin/Tan Verfahren 1 Schritt!
            // Infos hierzu: http://www.hbci-zka.de/dokumente/spezifikation_deutsch/fintsv3/FinTS_3.0_Security_Sicherheitsverfahren_HBCI_Rel_20130718_final_version.pdf Punkt B5.1
            // http://www.hbci-zka.de/dokumente/spezifikation_deutsch/fintsv3/FinTS_3.0_Security_Sicherheitsverfahren_PINTAN_Rel_20101027_final_version.pdf B8.4
            // Sicherheitsprofil ["PIN",1] = PIN und 1 Schrittverfahren
            // Sicherheitsfunktion: 999 - 1 SChrittverfahren / 2Schritt siehe BPD
            // Sicherheitskontrollreferenz: 1 // Muss mit Signaturabschluss übereinstimmen
            // Bereich der Sicherheitsapplikation,kodiert: 1 // 1: Signaturkopf und HBCI-Nutzdaten (SHM)
            // Rolle des Sicherheitslieferanten,kodiert: 1 // 1: Der Unterzeichner ist Herausgeber der signierten Nachricht, z.B. Erfasser oder Erstsignatur (ISS)
            // Sicherheitsidentifikation, Details: [1,null,0]
            //		Bezeichner Sicherheitspartei	1		1: Message Sender (MS), wenn ein Kunde etwas an sein Kreditinstitut sendet
            //		CID nur Chipkarte				null
            // 		Id der Partei nur Software		0		Code, welcher die (Kommunikations-)Partei identifiziert. Dieses Feld muss eine gültige, zuvor vom Banksystem angeforderte Kundensystem-ID enthalten (analog zum RSA-Verfahren). Dies gilt auch fürZweit-und Drittsignaturen.
            //			beim Erstmal noch 0, dann auf Antwort von Bank in HISYN warten und das verwenden!
            //	Sicherheitsreferenznummer: 1 Verhinderung der Doppeleinreichung Bei softwarebasierten Verfahren wird die Sicherheitsreferenznummer auf Basis des DE Kundensystem-ID und des DE Benutzerkennung der DEG Schlüsselnamen verwaltet.
            //							bei Pin/Tan kann das auch einfach bei 1 beibehalten werden :), sonst müsste man das aber eigtl. incrementieren
            // 	Sicherheitsdatum und -uhrzeit [1,"20141210","003515"], 1: Bedeutung = Sicherheitszeitstempel (STS)
            //	Hashalgorithmus: [1,999,1]
            //		Verwendung des Hashalgorithmus,kodiert	1: Owner Hashing (OHA) (nur)
            //		Hashalgorithmus,kodiert					999: Gegenseitig vereinbart (ZZZ); hier: RIPEMD-160 ( gibt noch andere Werte 1-6 vorallem SHAxxx
            // 		Bezeichner für Hashalgorithmusparameter	1: IVC (Initialization value, clear text)
            //	Signaturalgorithmus: [6,10,16]
            //		Verwendung des Signaturalgorithmus, kodiert 6: Owner Signing (OSG)
            //		10: RSA-Algorithmus (bei RAH und RDH)
            //		Operationsmodus, kodiert	16:	ISO 9796-1 (bei RDH)
            //	Schlüsselname	[280,blz,kunden_id,"S",0,0]
            //		Kreditinstitutskennung	280,blz
            //		Benutzerkennung 		kunden_id
            //		Schlüsselart			S	S: Signierschlüsse
            //		Schlüsselnummer			0
            //		Schlüsselversion		0
            var signature_id = (me_msg.sign_it.sys_id+"")=="0"?1:me_msg.sign_it.sig_id;
            me_msg.sign_it.blz = blz;
            me_msg.sign_it.kunden_id = kunden_id;
            var seg_vers,sec_profile;
            if(me_msg.proto_version==300){
                me_msg.sign_it.server===undefined?
                    me_msg.addSeg(Helper.newSegFromArray("HNSHK", 4, [["PIN",me_msg.sign_it.pin_vers=="999"?1:2],me_msg.sign_it.pin_vers,1,1,1,[1,NULL,me_msg.sign_it.sys_id],signature_id,[1,Helper.convertDateToDFormat(new Date()),Helper.convertDateToTFormat(new Date())],[1,999,1],[6,10,16],[280,blz,kunden_id,"S",0,0]]))
                    :me_msg.addSeg(Helper.newSegFromArray("HNSHK", 4, [["PIN",me_msg.sign_it.pin_vers=="999"?1:2],me_msg.sign_it.pin_vers,1,1,1,[2,NULL,me_msg.sign_it.sys_id],signature_id,[1,Helper.convertDateToDFormat(new Date()),Helper.convertDateToTFormat(new Date())],[1,999,1],[6,10,16],[280,blz,kunden_id,"S",0,0]]));
            }else{
                me_msg.sign_it.server===undefined?
                    me_msg.addSeg(Helper.newSegFromArray("HNSHK", 3, [me_msg.sign_it.pin_vers,1,1,1,[1,NULL,me_msg.sign_it.sys_id],signature_id,[1,Helper.convertDateToDFormat(new Date()),Helper.convertDateToTFormat(new Date())],[1,999,1],[6,10,16],[280,blz,kunden_id,"S",0,0]]))
                    :me_msg.addSeg(Helper.newSegFromArray("HNSHK", 3, [me_msg.sign_it.pin_vers,1,1,1,[2,NULL,me_msg.sign_it.sys_id],signature_id,[1,Helper.convertDateToDFormat(new Date()),Helper.convertDateToTFormat(new Date())],[1,999,1],[6,10,16],[280,blz,kunden_id,"S",0,0]]));
            }

        }
    };

    me_msg.parse = function (in_txt) {
        var parser = new Parser(in_txt);
        while (parser.hasNext()) {
            var segm = new Segment();
            segm.parse(parser);
            me_msg.segments.push(segm);
            parser.nextPos();
        }
        // prüfen ob verschlüsselt war
        if(me_msg.segments.length==4&&me_msg.segments[1].name == "HNVSK"&&me_msg.segments[2].name == "HNVSD"){
            var first     = me_msg.segments[0];
            me_msg.hnvsk  = me_msg.segments[1];
            var seg_hnvsd = me_msg.segments[2];
            var last      = me_msg.segments[3];
            // Neue Segmente hinzufügen
            me_msg.segments = new Array();
            me_msg.segments.push(first);
            if((me_msg.hnvsk.vers=="3"&&me_msg.hnvsk.getEl(1).getEl(1)=="PIN")||(me_msg.hnvsk.vers=="2"&&me_msg.hnvsk.getEl(1)=="998")){
                var parser2 = new Parser(seg_hnvsd.getEl(1));
                while (parser2.hasNext()) {
                    var segm2 = new Segment();
                    segm2.parse(parser2);
                    me_msg.segments.push(segm2);
                    parser2.nextPos();
                }
            }else{
                throw new ParseError("Msg", "Nicht unterstützte Verschlüsselungsmethode!", 0);
            }
            me_msg.segments.push(last);
        }
    };

    me_msg.transformForSend = function () {
        var top = me_msg.segments[0].transformForSend();
        var body = "";
        // Signatur abschluss
        if(me_msg.sign_it){
            // Signaturabschluss
            // Sicherheitskontrollreferenz 1 muss mit signaturkopf übereinstimmen
            // Validierungsresultat null, bleibt bei PinTan leer
            // Benutzerdefinierte Signatur [Pin,Tan], die Tan nur dann wenn durch den Geschäftsvorfall erforderlich
            if(me_msg.sign_it.server===undefined){
                if(me_msg.sign_it.tan===NULL){
                    me_msg.addSeg(Helper.newSegFromArray("HNSHA",me_msg.proto_version==300?2:1, [1,NULL,[me_msg.sign_it.pin]]));
                }else{
                    me_msg.addSeg(Helper.newSegFromArray("HNSHA", me_msg.proto_version==300?2:1, [1,NULL,[me_msg.sign_it.pin,me_msg.sign_it.tan]]));
                }
            }else{
                me_msg.addSeg(Helper.newSegFromArray("HNSHA", 2, [2]));
            }
        }
        for (var i = 1; i != me_msg.segments.length; i++) {
            body += me_msg.segments[i].transformForSend();
        }
        // Letztes segment erstellen
        if(me_msg.sign_it){
            // in body ist der eigentliche body der dann aber jetzt neu erstellt wird
            // Verschlüsselung
            // 1. HNVSK                                     HNVSK:998:3
            // Sicherheitsprofil                            [PIN:1]
            // Sicherheitsfunktion, kodiert                 998 // bleibt immer so unabhängig von der der tatsächlichen Funktion
            // Rolle des Sicherheits-lieferanten, kodiert   1
            // Sicherheitsidentifikation, Details           [1.null.0]
            // Sicherheitsdatum und -uhrzeit                [1,20141216,205751]
            // Verschlüsselungs-algorithmus                 [2,2,13,@8@,5,1]
            // Schlüsselname                                [280:12345678:max:V:0:0]
            //      Ctry Code                               280 (hier fest)
            //      BLZ
            //      benutzer
            //      Schlüsselart                            V Chiffrierschlüssel
            //      Schlüsselnummer                         0
            //      Schlüsselversion                        0
            // Komprimierungsfunktion                       0
            // Zertifikat                                   leer hier
            //+998+1+1::0+1:20141216:205751+2:2:13:@8@:5:1+280:12345678:max:V:0:0+0'
            if(me_msg.proto_version==300)
                me_msg.hnvsk = Helper.newSegFromArray("HNVSK", 3, [["PIN",me_msg.sign_it.pin_vers=="999"?1:2],998,1,[1,NULL,me_msg.sign_it.sys_id],[1,Helper.convertDateToDFormat(new Date()),Helper.convertDateToTFormat(new Date())],[2,2,13,Helper.Byte("\0\0\0\0\0\0\0\0"),5,1],[280,me_msg.sign_it.blz,me_msg.sign_it.kunden_id,"V",0,0],0]);
            else
                me_msg.hnvsk = Helper.newSegFromArray("HNVSK", 2, [998,1,[1,NULL,me_msg.sign_it.sys_id],[1,Helper.convertDateToDFormat(new Date()),Helper.convertDateToTFormat(new Date())],[2,2,13,Helper.Byte("\0\0\0\0\0\0\0\0"),5,1],[280,me_msg.sign_it.blz,me_msg.sign_it.kunden_id,"V",0,0],0]);
            me_msg.hnvsk.nr = 998;
            var seg_hnvsd = Helper.newSegFromArray("HNVSD", 1, [Helper.Byte(body)]);
            seg_hnvsd.nr = 999;
            body = me_msg.hnvsk.transformForSend();
            body+= seg_hnvsd.transformForSend();
        }
        // Abschließen
        var seg = Helper.newSegFromArray("HNHBS", 1, [me_msg.msg_nr]);
        me_msg.addSeg(seg);
        body += seg.transformForSend();
        var llength = top.length + body.length;
        me_msg.segments[0].store.data[0] = Helper.getNrWithLeadingNulls(llength, 12);
        top = me_msg.segments[0].transformForSend();
        return top + body;
    };
    me_msg.addSeg = function (seg) {
        seg.nr = me_msg.segments_ctr + 1;
        me_msg.segments[me_msg.segments_ctr] = seg;
        me_msg.segments_ctr++;
        return seg.nr;
    };

    me_msg.isSigned = function(){
        return   me_msg.selectSegByName("HNSHK").length==1;
    };

    me_msg.selectSegByName = function(name){
        var r = [];
        for(var i=0;i!=me_msg.segments.length;i++){
            if(me_msg.segments[i].name==name){
                r.push(me_msg.segments[i]);
            }
        }
        return r;
    };
    me_msg.selectSegByBelongTo = function(belong_to){
        var r = [];
        for(var i=0;i!=me_msg.segments.length;i++){
            if(me_msg.segments[i].bez==(belong_to+"")){
                r.push(me_msg.segments[i]);
            }
        }
        return r;
    };
    me_msg.selectSegByNameAndBelongTo = function(name,belong_to){
        var r = [];
        for(var i=0;i!=me_msg.segments.length;i++){
            if(me_msg.segments[i].name==name&&me_msg.segments[i].bez==(belong_to+"")){
                r.push(me_msg.segments[i]);
            }
        }
        return r;
    };
    // Nur für Debug/Entwicklungszwecke um ein JS Response aus einem echten Response zu generieren
    me_msg.create_debug_js = function(){
        var top = "var sendMsg = new FinTSClient().testReturnMessageClass();\n\r";
        var sig = "\n\r";
        var body = "";
        for(var i=0;i!=me_msg.segments.length;i++){
            if(me_msg.segments[i].name=="HNHBK"||
                me_msg.segments[i].name=="HNHBS"||
                me_msg.segments[i].name=="HNSHA"){
                // auslassen
            }else if(me_msg.segments[i].name=="HNSHK"){
                // Signatur
                sig = "sendMsg.sign({'pin':'pin1234','tan':null,'sys_id':'"+me_msg.segments[i].getEl(6).getEl(3)+"'});\n\r";
            }else{
                // generate array structure out of segment
                var seg_array = new Array();
                for(var a=0;a!=me_msg.segments[i].store.data.length;a++){
                    if(me_msg.segments[i].store.desc[a]==1){// DE
                        seg_array.push(me_msg.segments[i].store.data[a]);
                    }else if(me_msg.segments[i].store.desc[a]==2){//DEG
                        // DEG durchforsten
                        var deg_array = new Array();
                        for(var d=0;d!=me_msg.segments[i].store.data[a].data.length;d++){
                            if(me_msg.segments[i].store.data[a].desc[d]==1){// DE
                                deg_array.push(me_msg.segments[i].store.data[a].data[d]);
                            }else if(me_msg.segments[i].store.data[a].desc[d]==2){// DEG
                                //sollte hier garnicht auftreten
                                throw "FEHLER DEG erhalten wo dies nicht passieren sollte";
                            }else if(me_msg.segments[i].store.desc[a].desc[d]==3){//BINARY
                                deg_array.push("BYTE"+me_msg.segments[i].store.data[a].data[d]);
                            }
                        }
                        seg_array.push(deg_array);
                    }else if(me_msg.segments[i].store.desc[a]==3){//BINARY
                        seg_array.push("BYTE"+me_msg.segments[i].store.data[a]);
                    }
                }
                if(me_msg.segments[i].bez==0)
                    body += "sendMsg.addSeg(Helper.newSegFromArray('"+me_msg.segments[i].name+"', "+me_msg.segments[i].vers+", "+JSON.stringify(seg_array)+"));\n\r";
                else
                    body += "sendMsg.addSeg(Helper.newSegFromArrayWithBez('"+me_msg.segments[i].name+"', "+me_msg.segments[i].vers+","+me_msg.segments[i].bez+","+JSON.stringify(seg_array)+"));\n\r";
            }
        }
        return top+sig+body;
    };
};

var inheritsFrom = function (child, parent) {
    child.prototype = Object.create(parent.prototype);
};

var Exceptions = {};
Exceptions.OpenFinTSClientException = function(){
    Error.call(this); //super constructor
    Error.captureStackTrace(this, this.constructor);
};
inheritsFrom(Exceptions.OpenFinTSClientException, Error);
Exceptions.OpenFinTSClientException.prototype.toString = function(){
    return this.message?this.message:"OpenFinTSClientException";
};
Exceptions.GVNotSupportedByKI = function(type,avail){
    Exceptions.OpenFinTSClientException.call(this);
    this.gv_type = type;
    this.sp_vers = avail?[]:Object.keys(avail);
    this.message = "There is no version of "+this.gv_type+" which is supported by both, the client and the server.";
};
inheritsFrom(Exceptions.GVNotSupportedByKI, Exceptions.OpenFinTSClientException);
Exceptions.MalformedMessageFormat = function(msg){
    Exceptions.OpenFinTSClientException.call(this);
    this.message = "MalformedMessage: "+msg;
};
inheritsFrom(Exceptions.MalformedMessageFormat, Exceptions.OpenFinTSClientException);
Exceptions.OrderFailedException = function(msg){
    Exceptions.OpenFinTSClientException.call(this);
    this.msg_detail = msg;
    this.message = "Failed to perform Order, got error Message from Server.:"+msg.getEl(3);
};
inheritsFrom(Exceptions.OrderFailedException, Exceptions.OpenFinTSClientException);
Exceptions.InternalError = function(msg_txt){
    Exceptions.OpenFinTSClientException.call(this);
};
inheritsFrom(Exceptions.InternalError, Exceptions.OpenFinTSClientException);
Exceptions.GVFailedAtKI = function(msg){
    Exceptions.OpenFinTSClientException.call(this);
    this.data = msg;
    this.message = "GVFailed because Msg: "+this.data[0]+" - "+this.data[2];
};
inheritsFrom(Exceptions.GVFailedAtKI, Exceptions.OpenFinTSClientException);
Exceptions.ConnectionFailedException = function(hostname){
    Exceptions.OpenFinTSClientException.call(this);
    this.host = hostname;
    this.toString = function(){
        return "Connection to "+this.host+" failed.";
    };
};
inheritsFrom(Exceptions.ConnectionFailedException, Exceptions.OpenFinTSClientException);
/*
 .msg({ type:"",
 ki_type:"",
 send_msg:{
 1:[],
 2:[],
 3:function(){}
 },
 recv_msg:{
 1:function(seg_vers,relatedRespSegments,releatedRespMsgs,recvMsg)
 2:
 },
 aufsetzpunkt_loc:[]
 });
 .done(function(error,order,recvMsg){

 });
 */
// TODO implement TanVerfahren in Order
var Order = function(client){
    var me_order = this;
    me_order.client = client;
    me_order.error  = null;

    var int_req_tan = false;
    var int_send_msg = [];
    var int_gmsg_list = [];

    me_order.requireTan = function(){in_req_tan=true;};
    me_order.msg = function(in_data){
        // 0. check no error
        if(me_order.error){
            return false;
        }
        // 1. check if we support one of the segment versions
        var act_vers = 0;
        if( in_data.ki_type in client.bpd.gv_parameters ){
            var avail_vers = Object.keys(in_data.send_msg).sort(function(a, b){return b-a});
            for(var i in avail_vers){
                if(avail_vers[i] in client.bpd.gv_parameters[in_data.ki_type]){
                    act_vers = avail_vers[i];
                    break;
                }
            }
        }
        if(act_vers==0){
            me_order.error = new Exceptions.GVNotSupportedByKI(in_data.ki_type,client.bpd.gv_parameters[in_data.ki_type]);
            return false;
        }
        // 2. Find the appropriate action
        var act = null;
        if(typeof in_data.recv_msg === 'function'){
            act = in_data.recv_msg;
        }else if(act_vers in in_data.recv_msg){
            act = in_data.recv_msg[act_vers];
        }else if( 0 in in_data.recv_msg){
            act = in_data.recv_msg[0];
        }else{
            act = function(){};
        }
        // 3. Prepare the Send Message object
        int_send_msg.push({
            version:act_vers,
            segment:Helper.newSegFromArray(in_data.type,act_vers,in_data.send_msg[act_vers]),
            action:act,
            aufsetzpunkt:null,
            aufsetzpunkt_loc:(in_data.aufsetzpunkt_loc?in_data.aufsetzpunkt_loc:[]),
            finished:false,
            collected_segments:[],
            collected_messages:[]
        });
    };
    me_order.done = function(cb){
        // Exit CB is called when the function returns here it is checked if an error occures and then disconnects
        var exit_cb = function(error,order,recvMsg){
            if(error){
                me_order.client.MsgEndDialog(function(error2,recvMsg2){
                    if(error2){
                        me_order.client.log.con.error({error:error2},"Connection close failed after error.");
                    }else{
                        me_order.client.log.con.debug("Connection closed okay, after error.");
                    }
                });
            }
            cb(error,order,recvMsg);
        };
        // Main Part
        if(me_order.error){
            exit_cb(me_order.error,me_order,null);
        }else{
            // Message prepare
            var perform = function(){
                var msg = new Nachricht(me_order.client.proto_version);
                msg.sign({'pin':me_order.client.pin,'tan':NULL,'sys_id':me_order.client.sys_id,'pin_vers':me_order.client.upd.availible_tan_verfahren[0],'sig_id':me_order.client.getNewSigId()});
                msg.init(me_order.client.dialog_id, me_order.client.next_msg_nr,me_order.client.blz,me_order.client.kunden_id);
                me_order.client.next_msg_nr++;
                // Fill in Segments
                for(var j in int_send_msg){
                    if(!int_send_msg[j].finished){
                        // 1. Resolve Aufsetzpunkt if required, TODO here diferntiate between versions
                        if(int_send_msg[j].aufsetzpunkt){
                            if(int_send_msg[j].aufsetzpunkt_loc.length>=1){
                                for(;int_send_msg[j].segment.store.data.length<int_send_msg[j].aufsetzpunkt_loc[0];){
                                    int_send_msg[j].segment.store.addDE(NULL);
                                }
                                if(int_send_msg[j].aufsetzpunkt_loc.length<=1){
                                    // direkt
                                    int_send_msg[j].segment.store.data[int_send_msg[j].aufsetzpunkt_loc[0]-1]=int_send_msg[j].aufsetzpunkt;
                                }else{
                                    // Unter DEG
                                    exit_cb(new Exceptions.InternalError("Aufsetzpunkt Location is in DEG not supported yet."),me_order,null);
                                    return;
                                }
                            }else{
                                exit_cb(new Exceptions.InternalError("Aufsetzpunkt Location is not set but an aufsetzpunkt was delivered"),me_order,null);
                                return;
                            }
                        }
                        // 2. Add Segment
                        msg.addSeg(int_send_msg[j].segment);
                    }
                }
                // Send Segments to Destination
                me_order.client.SendMsgToDestination(msg,function(error,recvMsg){
                    if(error){
                        exit_cb(error,me_order,null);
                    }else{
                        var got_aufsetzpunkt = false;
                        // 1. global Message testen
                        var gmsg_exception = null;
                        try{
                            var HIRMG = recvMsg.selectSegByName("HIRMG")[0];
                            for(var i in HIRMG.store.data){
                                int_gmsg_list.push(HIRMG.store.data[i].data);
                                if(gmsg_exception==null&&HIRMG.store.data[i].data[0].charAt(0)=="9"){
                                    gmsg_exception = new Exceptions.OrderFailedException(HIRMG.store.data[i].data);
                                }
                            }
                        }catch(ee){exit_cb(new Exceptions.MalformedMessageFormat("HIRMG is mandatory but missing."),me_order,recvMsg);return;};
                        if(gmsg_exception!=null){
                            exit_cb(gmsg_exception,me_order,recvMsg);
                            return;
                        }
                        // 2. einzelne Resp Segmente durchgehen
                        try{
                            for(var j in int_send_msg){
                                var related_segments = recvMsg.selectSegByBelongTo(int_send_msg[j].segment.nr);
                                int_send_msg[j].finished = true;
                                for(var i in related_segments){
                                    if(related_segments[i].name=="HIRMS"){
                                        var HIRMS = related_segments[i];
                                        for(var a in HIRMS.store.data){
                                            int_send_msg[j].collected_messages.push(HIRMS.store.data[a].data);
                                            if(HIRMS.store.data[a].data[0]=="3040"){
                                                // Got an Aufsetzpunkt
                                                try{
                                                    int_send_msg[j].aufsetzpunkt = HIRMS.store.data[a].data[3];
                                                }catch(eee){int_send_msg[j].aufsetzpunkt = null;};
                                                int_send_msg[j].finished = false;
                                                got_aufsetzpunkt = true;
                                            }
                                        }
                                    }else{
                                        int_send_msg[j].collected_segments.push(related_segments[i]);
                                    }
                                }
                            }
                        }catch(ee){exit_cb(new Exceptions.InternalError("Failed parsing Segments"),me_order,recvMsg);};
                        // 3. check if we had an aufsetzpunkt
                        if(got_aufsetzpunkt){
                            perform();
                        }else{
                            // 4. Fertig die callbacks rufen
                            for(var j in int_send_msg){
                                int_send_msg[j].action(int_send_msg[j].version,int_send_msg[j].collected_segments,int_send_msg[j].collected_messages,recvMsg);
                            }
                            exit_cb(null,me_order,recvMsg);
                        }
                    }
                });
            };
            perform();
        }
    };
    me_order.checkMessagesOkay = function(messages,throw_when_error){
        for(var i in messages){
            var type = messages[i][0].charAt(0);
            if(type=="9"){
                if(throw_when_error){
                    Exceptions.GVFailedAtKI(messages[i]);
                }
                return false;
            }
        }
        return true;
    };
    me_order.getSegByName = function(list,name){
        for(var i in list){
            if(list[i].name==name)
                return list[i];
        }
        return null;
    };
    me_order.getElFromSeg = function(seg,nr,default_v){
        if(seg){
            var e = null;
            try{
                e = seg.getEl(nr);
            }catch(e2){
                e = default_v;
            }
            return e;
        }else{
            return default_v;
        }
    };
    me_order.checkKITypeAvailible = function(ki_type,vers,return_param){
        if( ki_type in me_order.client.bpd.gv_parameters ){
            var p_return = {};
            var test_vers = [];
            if(vers instanceof Array){
                test_vers = test_vers.concat(vers);
            }else{
                test_vers.push(vers);
            }
            for(var vindex in test_vers){
                if(test_vers[vindex] in me_order.client.bpd.gv_parameters[ki_type]){
                    if(return_param){
                        p_return[vindex] = me_order.client.bpd.gv_parameters[ki_type][test_vers[vindex]];
                    }else{
                        return true;
                    }
                }
            }
            if(return_param){
                return p_return;
            }else{
                return false;
            }
        }else{
            if(return_param){
                return {};
            }else{
                return false;
            }
        }
    };
};
function OrderHelperChain(){
    this.returner = {};
};
OrderHelperChain.prototype.vers = function(v,cb){
    if(v instanceof Array){
        for(var i in v){
            this.returner[v[i]]=cb;
        }
    }else if(v){
        this.returner[v]=cb;
    }else{
        throw new Error("Development Error "+v+" not defined");
    }
    return this;
};
OrderHelperChain.prototype.done = function(){
    return this.returner;
};
Order.prototype.Helper = function(){
    return new OrderHelperChain();
};
var beautifyBPD = function(bpd){
    var cbpd = bpd.clone();
    cbpd.gv_parameters = "...";
    return cbpd;
};

var MTParser = function(){
    var me = this;
    me.msgss = [];

    me.parse = function(txt){
        var cur_msg = [];
        var msgs = [];
        var parser = new Parser(txt);

        while(parser.hasNext()){
            if(parser.gotoNextValidChar(":")){
                parser.nextPos();
                parser.setMarkerWithCurrentPos("start");
                parser.gotoNextValidChar(":");
                var tag = parser.getTextFromMarkerToCurrentPos("start");
                parser.nextPos();
                parser.setMarkerWithCurrentPos("start");
                parser.gotoNextValidChar("\n\r");
                var val = parser.getTextFromMarkerToCurrentPos("start");
                parser.nextPos();
                parser.nextPos();
                while(parser.hasNext()&&!(parser.getCurrentChar()==":"||parser.getCurrentChar()=="-")){
                    parser.setMarkerWithCurrentPos("start");
                    parser.gotoNextValidChar("\n\r");
                    val += parser.getTextFromMarkerToCurrentPos("start");
                    parser.nextPos();
                    parser.nextPos();
                }
                cur_msg.push([tag,val]);
            }
            // schauen ob die Message zuende ist
            if(parser.getCurrentChar()=="-"&&(parser.cur_pos+1>=parser.data.length||parser.data[parser.cur_pos+1]=="\r")){
                msgs.push(cur_msg);
                cur_msg = [];
                parser.nextPos();
                parser.nextPos();
                parser.nextPos();
            }
        }
        if(cur_msg.length>0){
            msgs.push(cur_msg);
        }
        // 1. Phase des Parsens beendet
        me.msgss = msgs;
    };

    me.getKontoUmsaetzeFromMT490 = function(){
        var umsaetze = [];
        for(var i=0;i!=me.msgss.length;i++){
            var msg = me.msgss[i];
            var umsatz = {};
            // Starten
            for(var a=0;a!=msg.length;a++){
                switch(msg[a][0]){
                    case "20":
                        umsatz.refnr = msg[a][1];
                        break;
                    case "21":
                        umsatz.bez_refnr = msg[a][1];
                        break;
                    case "25":
                        umsatz.konto_bez = msg[a][1];
                        break;
                    case "28C":
                        umsatz.auszug_nr = msg[a][1];
                        break;
                    case "60F":// Anfangssaldo
                    case "60M":// Zwischensaldo
                        me.parseMT490_60a(umsatz,msg,a);
                        break;
                    case "61":// Loop
                        a = me.parseMT490_loop(umsatz,msg,a);
                        break;
                    case "62F":// Endsaldo
                    case "62M":// Zwischensaldo
                        me.parseMT490_62a(umsatz,msg,a);
                        break;
                }
            }
            umsaetze.push(umsatz);
        }
        return umsaetze;
    };

    me.parseMT490_60a = function(umsatz,msg,a){
        var string 							= msg[a][1];
        umsatz.anfangssaldo 				= {};
        umsatz.anfangssaldo.isZwischensaldo	= msg[a][0][2]=="M";
        umsatz.anfangssaldo.soll_haben 		= string[0]=="C"?"H":"S";
        umsatz.anfangssaldo.buchungsdatum 	= me.convertMTDateFormatToJS(string.substr(1,6));
        umsatz.anfangssaldo.currency	  	= string.substr(7,3);
        umsatz.anfangssaldo.value			= parseFloat(string.substr(10,string.length).replace(',', '.'));
    };
    me.parseMT490_62a = function(umsatz,msg,a){
        var string 							= msg[a][1];
        umsatz.schlusssaldo 				= {};
        umsatz.schlusssaldo.isZwischensaldo	= msg[a][0][2]=="M";
        umsatz.schlusssaldo.soll_haben 		= string[0]=="C"?"H":"S";
        umsatz.schlusssaldo.buchungsdatum 	= me.convertMTDateFormatToJS(string.substr(1,6));
        umsatz.schlusssaldo.currency	  	= string.substr(7,3);
        umsatz.schlusssaldo.value			= parseFloat(string.substr(10,string.length).replace(',', '.'));
    };

    me.parseMT490_loop = function(umsatz,msg,a){
        umsatz.saetze = [];
        for(;a<msg.length&&msg[a][0]=="61";a++){
            var satz = {};
            var pos  = 0;
            // 1. 61
            satz.datum = me.convertMTDateFormatToJS(msg[a][1].substr(0,6));
            if("0123456789".indexOf(msg[a][1][6])!=-1){
                // optionales feld Buchungstag
                pos = 10;
            }else{
                pos = 6;
            }
            if(msg[a][1][pos]=="R"){
                satz.is_storno = true;
                pos+1;
            }else{
                satz.is_storno = false;
            }
            satz.soll_haben = msg[a][1][pos]=="C"?"H":"S";
            pos++;
            if("0123456789".indexOf(msg[a][1][pos])==-1){
                // optionales feld Währungsunterscheidung
                pos++;
            }else{

            }
            // Betrag
            var start_pos = pos;
            var end_pos   = pos;
            for(var j=start_pos;j<msg[a][1].length;j++){
                if(msg[a][1][j]=="N"){
                    end_pos = j;
                    break;
                }
            }
            satz.value = parseFloat(msg[a][1].substring(start_pos,end_pos).replace(',', '.'));
            pos = end_pos+1;
            // 2. 86
            a++;
            me.parseMT490_86(satz,msg[a][1]);
            // TODO hier gibt es auch noch eine weiter bearbeitung
            umsatz.saetze.push(satz);
        }
        a--;
        return a;
    };

    me.parseMT490_86 = function(satz,raw_verwen_zweck){
        satz.is_verwendungszweck_object = raw_verwen_zweck[0]=="?"||raw_verwen_zweck[1]=="?"||raw_verwen_zweck[2]=="?"||raw_verwen_zweck[3]=="?";
        if(satz.is_verwendungszweck_object){
            satz.verwendungszweck = {};
            satz.verwendungszweck.text = "";
            var p = new Parser(raw_verwen_zweck);
            p.gotoNextValidChar("?");
            while(p.hasNext()){
                // Hier sind wir immer auf einem ?
                p.nextPos();
                p.setMarkerWithCurrentPos("start");
                p.nextPos();
                p.nextPos();
                var code  = p.getTextFromMarkerToCurrentPos("start");
                p.setMarkerWithCurrentPos("start");
                p.gotoNextValidChar("?");
                var value = p.getTextFromMarkerToCurrentPos("start");
                // Processing
                switch(code){
                    case "00":
                        satz.verwendungszweck.buchungstext = value;
                        break;
                    case "10":
                        satz.verwendungszweck.primanoten_nr = value;
                        break;
                    case "20":
                    case "21":
                    case "22":
                    case "23":
                    case "24":
                    case "25":
                    case "26":
                    case "27":
                    case "28":
                    case "29":
                    case "60":
                    case "61":
                    case "62":
                    case "63":
                        satz.verwendungszweck.text += value;
                        break;
                    case "30":
                        satz.verwendungszweck.bic_kontrahent = value;
                        break;
                    case "31":
                        satz.verwendungszweck.iban_kontrahent = value;
                        break;
                    case "32":
                        satz.verwendungszweck.name_kontrahent = value;
                        break;
                    case "34":
                        satz.verwendungszweck.text_key_addion = value;
                        break;
                }

            }
        }else{
            satz.verwendungszweck = raw_verwen_zweck;
        }
    };


    me.convertMTDateFormatToJS = function(date){
        var result = new Date();
        result.setTime(0);
        result.setYear(parseInt("20"+date.substr(0,2),10));
        result.setMonth(parseInt(date.substr(2,2),10)-1);
        result.setDate(parseInt(date.substr(4,2),10));
        return result;
    };
};


/*
 FinTSClient(in_blz,in_kunden_id,in_pin,in_logger)
 in_blz			- Die entsprechende BLZ als Zahl oder String
 in_kunden_id	- Die Benutzerkennung bzw. Kunden-ID - 9999999999 = Anonymer Benutzer
 in_pin			- Die Pin
 in_logger		- Ein Bunyan Logger per default wird nichts gelogged

 Attribute
 = Notwendig um die Verbindung herzustellen =
 blz
 ctry			- Zurzeit immer 280 für Deutschland
 kunden_id		-
 pin				-
 tan				- Noch NULL, da keine Geschäftsvorfälle mit Tan zurzeit unterstützt
 debug_mode		- Debug Modus (Logging) sollte nicht genutzt werden, kann über ein bunyan logger auch realisiert werden

 = Status des aktuellen Client Objekts =
 dialog_id		- Ein FinTSClient Objekt repräsentiert ein Dialog / dies ist die vom KI zugewiesene ID
 next_msg_nr		- Nachrichten werden Nummeriert beginnend von 1 dies ist die nächste Nummer
 client_name		- Name des Clients, sollte an die entsprechende benutztende Software angepasst werden
 client_version	- Version des Clients

 = Bank Paramter Daten und System-ID + letzte benutzte Signatur-ID
 sys_id			- vom KI zugewiesene System-ID, identifiziert diese Anwendung für den entsprechenden Benutzer eindeutig.
 Sollte um unnötige Anlage neuer IDs zu Vermeiden für weitere Verbindungen beibehalten werden (für immer).
 last_signatur_id - Zuletzt verwendete Signatur-ID hängt an der System-ID und gewährleistet, dass Nachrichten nicht mehrfach eingereicht werden.
 bpd				- Die Bank Paramter Daten siehe Dokumentation zu mehr Details
 {
 'vers_bpd':"0",									// Version der BPD
 'bank_name':"",									// Name der Bank
 'supported_vers':["300"],						// Unterstützte HBCI/FinTS Versionen
 'url':"",										// URL für Pin/Tan, wird durch die Bankenliste und die BLZ vorbelegt
 'pin':{
 'min_length':0,								// Minimal Länge der Pin
 'max_length':100,							// Maximal Länge der Pin
 'max_tan_length':100,						// Maximale Länger der Tan
 'txt_benutzerkennung':'Benutzerkennung',	// Vorbelegungs Text für das Feld Benutzerkennung
 'txt_kunden_id':'Kunden ID',				// Vorbelegungs Text für das Feld Kunden-ID
 'availible_seg':{							// Verfügbare Geschäftsvorfälle als Key und Wert für Tanerforderlichkeit
 'HXXXX':true,								// Wert true -> mit Tan
 'HXXXX':false								// Wert false -> ohne Tan
 }
 },
 'tan':{
 'one_step_availible':true,					// Ein-Schritt-Verfahren verfügbar
 'multiple_tan':false,						// Mehrfachtan
 'hash_type':"0",							// zu verwendender Hash Algorhytmus
 'tan_verfahren':{'999':{					// Verfügbare Tan Verfahren
 'code':'999',								// Code des Verfahrens
 'one_two_step_vers':"1",					// 1-Ein Schritt-Verfahren / 2-Zwei Schritt-Verfahren
 'tech_id':'PIN',							// Technische ID des Verfahrens
 'desc':'Einfaches Pin-Verfahren',			// Lesbare Beschreibung des Verfahrens
 'max_len_tan':100,							// Maximal Länge der Tan
 'tan_alphanum':true,						// Tan Alphanumerisch?
 'txt_rueckwert':'Rückgabewert',				// Vorbelegungs Text Rückgabewert
 'max_len_rueckwert':100,					// Maximale Länge des Rückgabewerts
 'anz_tanlist':'2',							// Anzahl Tan-Listen
 'multi_tan':true,							// Mehrfachtan?
 'tan_zeit_diabez':"",						// Tan Zeit Dialog Bezug
 'tan_list_nr_req':"",						// Tan Listennummer erforderlich?
 'auftragsstorno':false,						// Auftragsstorno?
 'challange_class_req':false,				// Challange Klasse erforderlich?
 'challange_value_req':false					// Challange Wert erforderlich?
 }}
 },
 'clone':function()								// Funktion um die Daten zu Clonen
 'gv_parameters':{
 "HHHH":{
 1:SEGMENT,
 2:SEGMENT
 }
 }
 };

 = User Paramter Daten =
 upd				- Die User Paramter Daten
 {
 'vers_upd':"0",										// Version der User Paramter Daten
 'geschaefts_vorg_gesp':true,						// Wie sind die nicht aufgeführten Geschäftsvorfälle zu Werten? true =  sind gesperrt / false = Keine Aussage darüber treffbar
 'availible_tan_verfahren':["999"],					// Verfügbare Tan Verfahren für den Benutzer, [0] ist die aktuell verwendete
 'clone':function()									// Funktion um die Daten zu Clonen
 };
 konten 			- Liste der Konten des Benutzers
 [{
 'iban':"", 					// IBAN des Kontos
 'konto_nr': 				// Konto-Nr
 'unter_konto': 				// Unterkonto Merkmal
 'ctry_code': 				// Länderkennzeichen idr. 280 für Deutschland
 'blz': 						// BLZ
 'kunden_id': 				// Kunden ID dem das Konto gehört
 'kontoar':	 				// Art des Kontos
 'currency': 				// Währung des Kontos
 'kunde1_name': 				// Name des Kunden
 'product_name': 			// Produktbezeichnung
 'sepa_data':{				// Zusätzliche Daten für SEPA Konten, kann null sein, wenn kein SEPA Konto z.B. Depots etc.
 'is_sepa': true,			// Ist SEPA Konto?
 'iban':"",					// IBAN
 'bic':"",					// BIC
 'konto_nr':"",				// Konto_NR
 'unter_konto':"",			// Unter Konto
 'ctry_code':"280",			// Ctry Code
 'blz':""					// BLZ
 }
 }]

 Methoden
 <-- Internal -->
 clear()								-	Initialisiert alle Attribute
 getNewSigId()						-	Erzeugt eine neue Signatur ID
 returns sig_id (int)
 SendMsgToDestination(msg,callback)	-	Verschickt Nachricht per HTTPS an die Bank
 msg		(Nachricht)
 callback (function(error,msg))	=	Wird gerufen wenn Nachricht erfolgreich (error==null) verschickt + Antwort(msg instance of Nachricht) empfangen
 debugLogMsg(txt,send)				- Zum Loggen von Nachrichten

 <-- Public -->
 MsgInitDialog(callback)				- Initialisiert einen Dialog
 callback (function(error,recvMsg,has_neu_url))	- error == null Kein Fehler
 - recvMsg (Nachricht)
 - has_neu_url == true wenn eine andere URL zurückgemeldet wurde
 MsgEndDialog(callback)				- Beendet einen Dialog
 callback (function(error,recvMsg))				- error == null kein Fehler
 - recvMsg (Nachricht)
 EstablishConnection(callback)		- Vereinfachte Variante um eine Verbindung mit der Bank aufzubauen
 callback (function(error))						- error == null kein Fehler
 != null Fehler / ein MsgEndDialog ist nichtmehr erforderlich
 MsgRequestSepa(for_konto_nr,callback) - Lade SEPA Zusatz Daten (vor allem die BIC)
 for_konto_nr									- Konto-Nr für das betreffende Konto, kann aber auch weg gelassen werden, dann für alle Konten
 callback (function(error,recvMsg,sepa_list))	-error == null kein Fehler
 -recvMsg (Nachricht)
 -sepa_list [] array von Sepa Daten Format siehe UPD Konten[].sepa_data
 MsgGetKontoUmsaetze(konto,from_date,to_date,callback) - Lädt die Kontenumsätze für ein bestimmtes Konto
 konto											- Das Konto für das die Umsätze geladen werden sollen
 from_date (Date)								- vom Datum (können leer==null gelassen werden dann wird alles verfügbare geladen)
 to_date	  (Date)								- zum Datum
 callback  (function(error,recvMsg,umsaetze))	- error == null kein Fehler
 - recvMsg (Nachricht)
 - umsaetze [] Enthält die Umsatz Daten mit folgendem Format
 [{			// pro Tag ein Objekt siehe MT490 SWIFT Format
 'refnr':"STARTUMS",		// ReferenzNummer
 'bez_refnr':null,		// BezugsreferenzNummer
 'konto_bez':"12345678/0000000001",	// Kontobezeichnung BLZ/Kontonr
 'auszug_nr':"",			// Auszugsnummer
 'anfangssaldo':{
 'isZwischensaldo':false,
 'soll_haben' 	: 'H',
 'buchungsdatum' : Date,
 'currency':'EUR',
 'value':150.22 },
 'schlusssaldo':{
 'isZwischensaldo':false,
 'soll_haben' 	: 'H',
 'buchungsdatum' : Date,
 'currency':'EUR',
 'value':150.22 },
 'saetze':[				// Die eigentlichen Buchungssätze
 {
 'datum':Date,
 'is_storno':false,
 'soll_haben':'S',
 'value':150.22,
 'is_verwendungszweck_object':true,// Verwendungszweck ist Objekt?
 'verwendungszweck': "TEXT" // oder
 {
 'buchungstext':"",
 'primanoten_nr':"",
 'text':"",
 'bic_kontrahent':"",
 'iban_kontrahent':"",
 'name_kontrahent':"",
 'text_key_addion':""
 }
 }
 ]
 }]


 MsgGetSaldo(konto,cb)			-Lädt den Saldo eines bestimmten Kontos
 konto											- Das Konto für das der Saldo geladen werden sollen
 callback  (function(error,recvMsg,saldo))		- error == null kein Fehler
 -saldo
 {"desc":"Normalsparen",
 "cur":"EUR",
 "saldo":{	"soll_haben":"H",		// SALDO OBJECT
 "buchungsdatum":Date,
 "currency":"EUR",
 "value":5},
 "saldo_vorgemerkt":null,			// SALDO OBJECT
 "credit_line":{ 	"currency":"EUR",
 "value":5},		// BETRAG OBJECT
 "avail_amount":null,				// BETRAG OBJECT
 "used_amount":null,				// BETRAG OBJECT
 "overdraft":null,					// BETRAG OBJECT
 "booking_date":Date,
 "faelligkeit_date":Date}
 closeSecure ()			-	Stellt sicher, dass keine Sensiblen Informationen wie die PIN noch im RAM sind, sollte am Ende immer gerufen werden

 */




var FinTSClient = function (in_blz,in_kunden_id,in_pin,bankenlist) {
    var me 					= this;
    me.Exceptions			= Exceptions;
    // Logger
    me.log					= {
        'main': new logger()
    };
    me.log.con	=	me.log.main.child({area: 'connection'});
    me.log.conest	=	me.log.main.child({area: 'connection_establish'});
    me.log.gv	=	me.log.main.child({area: 'gv'});
    // Other
    me.blz 					= in_blz;
    me.ctry 				= 280;
    me.kunden_id   			= in_kunden_id;
    me.pin		   			= in_pin;
    me.tan		   			= NULL;
    me.debug_mode			= false;

    // Technical
    me.dialog_id   			= 0;
    me.next_msg_nr 			= 1;
    me.client_name 			= "Open-FinTS-JS-Client";
    me.client_version 		= 4;
    me.proto_version		= 300;// 300=FinTS;220=HBCI 2.2

    // BPD und System-Id mit Letzt benutzter Signatur ID
    me.sys_id				= 0;
    me.last_signatur_id 	= 1;
    me.bpd					= {	};
    me.bpd.url 				= "";

    // UPD - Data
    me.upd = {	};
    me.konten = [];

    me.clear = function(){
        me.dialog_id    = 0;
        me.next_msg_nr  = 1;
        me.sys_id		= 0;
        me.last_signatur_id = 1;
        me.bpd			={
            'vers_bpd':"0",
            'bank_name':"",
            'supported_vers':["300"],
            'url':"",
            'pin':{
                'min_length':0,
                'max_length':100,
                'max_tan_length':100,
                'txt_benutzerkennung':'Benutzerkennung',
                'txt_kunden_id':'Kunden ID',
                'availible_seg':{}
            },
            'tan':{
                'one_step_availible':true,
                'multiple_tan':false,
                'hash_type':"0",
                'tan_verfahren':{'999':{
                    'code':'999',
                    'one_two_step_vers':"1",
                    'tech_id':'PIN',
                    'desc':'Einfaches Pin-Verfahren',
                    'max_len_tan':100,
                    'tan_alphanum':true,
                    'txt_rueckwert':'Rückgabewert',
                    'max_len_rueckwert':100,
                    'anz_tanlist':'2',
                    'multi_tan':true,
                    'tan_zeit_diabez':"",
                    'tan_list_nr_req':"",
                    'auftragsstorno':false,
                    'challange_class_req':false,
                    'challange_value_req':false
                }}
            },
            'clone':function(){return JSON.parse(JSON.stringify(this));},
            'gv_parameters':{}

        };
        me.bpd.url = bankenlist[""+in_blz].url;

        me.upd = {
            'vers_upd':"0",
            'geschaefts_vorg_gesp':true,
            'availible_tan_verfahren':["999"],
            'clone':function(){return JSON.parse(JSON.stringify(this));}
        };
        me.konten = [];
    };
    me.clear();

    me.closeSecure = function(){
        me.bpd = null;
        me.upd = null;
        me.konten = null;
        me.pin = null;
        me.tan = null;
        me.sys_id = null;
    };

    me.getNewSigId = function(){
        var next = (new Date()).getTime();
        if(next>me.last_signatur_id){
            me.last_signatur_id = next;
            return me.last_signatur_id;
        }else{
            me.last_signatur_id++;
            return me.last_signatur_id;
        }
    };

    me.MsgInitDialog = function(cb){
        var msg = new Nachricht(me.proto_version);
        if(me.kunden_id!=9999999999)msg.sign({'pin':me.pin,'tan':NULL,'sys_id':me.sys_id,'pin_vers':me.upd.availible_tan_verfahren[0],'sig_id':me.getNewSigId()});
        msg.init(me.dialog_id, me.next_msg_nr,me.blz,me.kunden_id);
        me.next_msg_nr++;
        //  Kundensystem-ID  = 0; Kundensystemssatus = 0
        msg.addSeg(Helper.newSegFromArray("HKIDN", 2, [[me.ctry, me.blz], me.kunden_id , me.sys_id, 1]));
        // BPD Vers = 0; UPD Vers = 0; Dialogspr. = 0
        var HKVVB = Helper.newSegFromArray("HKVVB", 3, [me.bpd.vers_bpd, me.upd.vers_upd, 0, me.client_name, me.client_version]);
        msg.addSeg(HKVVB);
        if(me.kunden_id!=9999999999&&me.sys_id==0)var syn=msg.addSeg(Helper.newSegFromArray("HKSYN",me.proto_version==220?2:3,[0]));// Synchronisierung starten
        me.log.gv.debug({gv:"HKVVB"},"Send HKVVB,HKIDN");
        me.SendMsgToDestination(msg,function(error,recvMsg){
            if(error){
                me.log.gv.error(error,{gv:"HKVVB"},"Could not send HKVVB,HKIDN");
                try{
                    cb(error,recvMsg,false);
                }catch(cb_error){
                    me.log.gv.error(cb_error,{gv:"HKVVB"},"Unhandled callback Error in HKVVB,HKIDN");
                }
            }else{
                // Prüfen ob Erfolgreich
                var HIRMG = null;
                try{HIRMG = recvMsg.selectSegByName("HIRMG")[0];}catch(e){};
                if(HIRMG!=null&&(HIRMG.getEl(1).getEl(1)=="0010"||HIRMG.getEl(1).getEl(1)=="3060")){
                    if(Helper.checkMsgsWithBelongToForId(recvMsg,HKVVB.nr,"0020")){
                        try{
                            // 1. Dialog ID zuweisen
                            me.dialog_id = recvMsg.selectSegByName("HNHBK")[0].getEl(3);
                            // 2. System Id
                            if(me.kunden_id!=9999999999&&me.sys_id==0){
                                me.sys_id = recvMsg.selectSegByNameAndBelongTo("HISYN",syn)[0].getEl(1);
                            }
                            // 3. Möglicherweise neue kommunikationsdaten
                            var HIKOM = recvMsg.selectSegByName("HIKOM");
                            HIKOM = HIKOM.length>0?HIKOM[0]:null;
                            var neu_url = me.bpd.url;
                            if(HIKOM){
                                for(var i=2; i<HIKOM.store.data.length;i++){
                                    // There can be up to 9 Kommunikationsparameter
                                    //  however we check only if the first one which is HTTP (3)
                                    //	is different to the one we used before, according to the spec we should try reconnecting all 9
                                    if(HIKOM.store.data[i].getEl(1)=="3"){
                                        neu_url = (Helper.convertFromToJSText(HIKOM.store.data[i].getEl(2)));
                                        if(neu_url.indexOf("http")!=0){
                                            neu_url = "https://"+neu_url;
                                        }
                                        break;
                                    }
                                }
                            }
                            var has_neu_url = false;
                            if(neu_url!=me.bpd.url){
                                has_neu_url = true;
                            }
                            // 4. Mögliche KontoInformationen
                            if(me.konten.length==0){
                                var konto_list = recvMsg.selectSegByName("HIUPD");
                                for(var i=0;i!=konto_list.length;i++){
                                    var konto = new Konto();
                                    konto.iban 			= konto_list[i].getEl(2);
                                    konto.konto_nr 		= konto_list[i].getEl(1).getEl(1);
                                    konto.unter_konto 	= konto_list[i].getEl(1).getEl(2);
                                    konto.ctry_code 	= konto_list[i].getEl(1).getEl(3);
                                    konto.blz 			= konto_list[i].getEl(1).getEl(4);
                                    konto.kunden_id 	= konto_list[i].getEl(3);
                                    konto.kontoar	 	= konto_list[i].getEl(4);
                                    konto.currency 		= konto_list[i].getEl(5);
                                    konto.kunde1_name 	= konto_list[i].getEl(6);
                                    konto.product_name 	= konto_list[i].getEl(8);
                                    konto.sepa_data		= null;
                                    me.konten.push(konto);
                                }
                            }
                            // 5. Analysiere BPD
                            try{
                                // 5.1 Vers
                                var HIBPA 			= recvMsg.selectSegByName("HIBPA")[0];
                                me.bpd.vers_bpd  	= HIBPA.getEl(1);
                                // 5.2 sonst
                                me.bpd.bank_name 		= HIBPA.getEl(3);
                                me.bpd.supported_vers 	= Helper.convertIntoArray(HIBPA.getEl(6));
                                me.bpd.url 				= neu_url;
                            }catch(ee){me.log.gv.error(ee,{gv:"HIBPA"},"Error while analyse BPD");}
                            if(me.proto_version==300){
                                try{
                                    // 5.3 Pins
                                    var pin_data = recvMsg.selectSegByName("HIPINS")[0].getEl(4);
                                    me.bpd.pin.min_length 			= pin_data.getEl(1);
                                    me.bpd.pin.max_length 			= pin_data.getEl(2);
                                    me.bpd.pin.max_tan_length 		= pin_data.getEl(3);
                                    me.bpd.pin.txt_benutzerkennung  = pin_data.getEl(4);
                                    me.bpd.pin.txt_kunden_id 		= pin_data.getEl(5);
                                    // 5.3.2 Tanerforderlichkeit für die Geschäftsvorfälle
                                    me.bpd.pin.availible_seg = {};// true and false für ob Tan erforderlich
                                    for(var i=5;i<pin_data.data.length;i++){
                                        me.bpd.pin.availible_seg[pin_data.data[i]]=pin_data.data[i+1].toUpperCase()=="J";
                                        i++;
                                    }
                                }catch(ee){me.log.gv.error(ee,{gv:"HIPINS"},"Error while analyse HIPINS");}
                            }else{
                                var pin_data_spk = recvMsg.selectSegByName("DIPINS");
                                if(pin_data_spk.length>0){
                                    try{
                                        // 5.3 Pins
                                        pin_data_spk 					= pin_data_spk[0];
                                        /*me.bpd.pin.min_length 		= ;
                                         me.bpd.pin.max_length 			= ;
                                         me.bpd.pin.max_tan_length 		= ;
                                         me.bpd.pin.txt_benutzerkennung  = ;
                                         me.bpd.pin.txt_kunden_id 		= ;*/
                                        // 5.3.2 Tanerforderlichkeit für die Geschäftsvorfälle
                                        me.bpd.pin.availible_seg = {};// true and false für ob Tan erforderlich
                                        var pin_tan_spk_data = pin_data_spk.getEl(3).data;
                                        for(var i=0;i<pin_tan_spk_data.length;i++){
                                            me.bpd.pin.availible_seg[pin_tan_spk_data[i]]=pin_tan_spk_data[i+1].toUpperCase()=="J";
                                            i++;
                                        }
                                    }catch(ee){me.log.gv.error(ee,{gv:"DIPINS"},"Error while analyse HIPINS");}
                                }else{
                                    me.log.gv.warning({gv:"HIPINS"},"Becuase it is 2.2 no HIPINS and no DIPINS.");
                                }
                            }
                            try{
                                // 5.4 Tan
                                var HITANS = recvMsg.selectSegByName("HITANS")[0];
                                if(HITANS.vers==5){
                                    var tan_data = HITANS.getEl(4);
                                    me.bpd.tan.one_step_availible = tan_data.getEl(1).toUpperCase()=="J";
                                    me.bpd.tan.multiple_tan	      = tan_data.getEl(2).toUpperCase()=="J";
                                    me.bpd.tan.hash_type	      = tan_data.getEl(3);
                                    me.bpd.tan.tan_verfahren = {};
                                    for(var i=3;i<tan_data.data.length;i++){
                                        var sicherheitsfunktion = {};
                                        sicherheitsfunktion.code 				= tan_data.data[i];
                                        sicherheitsfunktion.one_two_step_vers 	= tan_data.data[i+1];// "1": Einschrittverfahren, "2": Zweischritt
                                        sicherheitsfunktion.tech_id				= tan_data.data[i+2];
                                        sicherheitsfunktion.zka_tan_verfahren	= tan_data.data[i+3];
                                        sicherheitsfunktion.vers_zka_tan_verf	= tan_data.data[i+4];
                                        sicherheitsfunktion.desc				= tan_data.data[i+5];
                                        sicherheitsfunktion.max_len_tan			= tan_data.data[i+6];
                                        sicherheitsfunktion.tan_alphanum		= tan_data.data[i+7]=="2";
                                        sicherheitsfunktion.txt_rueckwert		= tan_data.data[i+8];
                                        sicherheitsfunktion.max_len_rueckwert	= tan_data.data[i+9];
                                        sicherheitsfunktion.anz_tanlist			= tan_data.data[i+10];
                                        sicherheitsfunktion.multi_tan			= tan_data.data[i+11].toUpperCase()=="J";
                                        sicherheitsfunktion.tan_zeit_diabez		= tan_data.data[i+12];
                                        sicherheitsfunktion.tan_list_nr_req		= tan_data.data[i+13];
                                        sicherheitsfunktion.auftragsstorno		= tan_data.data[i+14].toUpperCase()=="J";
                                        sicherheitsfunktion.sms_abu_konto_req	= tan_data.data[i+15];
                                        sicherheitsfunktion.auftrag_konto		= tan_data.data[i+16];
                                        sicherheitsfunktion.challange_class_req = tan_data.data[i+17].toUpperCase()=="J";
                                        sicherheitsfunktion.challange_structured = tan_data.data[i+18].toUpperCase()=="J";
                                        sicherheitsfunktion.initialisierungs_mod = tan_data.data[i+19];
                                        sicherheitsfunktion.bez_tan_med_req		 = tan_data.data[i+20];
                                        sicherheitsfunktion.anz_supported_tan_vers = tan_data.data[i+21];
                                        //sicherheitsfunktion.challange_value_req = tan_data.data[i+14].toUpperCase()=="J";
                                        me.bpd.tan.tan_verfahren[sicherheitsfunktion.code]=sicherheitsfunktion;
                                        i+=21;
                                    }
                                }
                            }catch(ee){me.log.gv.error(ee,{gv:"HITANS"},"Error while analyse HITANS");}
                            // 6. Analysiere UPD
                            try{
                                var HIUPA = recvMsg.selectSegByName("HIUPA")[0];
                                me.upd.vers_upd = HIUPA.getEl(3);
                                me.upd.geschaefts_vorg_gesp = HIUPA.getEl(4)=="0"; // UPD-Verwendung
                            }catch(ee){me.log.gv.error(ee,{gv:"HIUPA"},"Error while analyse UPD");}
                            // 7. Analysiere Verfügbare Tan Verfahren
                            try{
                                var HIRMS_for_tanv = recvMsg.selectSegByNameAndBelongTo("HIRMS",HKVVB.nr)[0];
                                for(var i=0;i!=HIRMS_for_tanv.store.data.length;i++){
                                    if(HIRMS_for_tanv.store.data[i].getEl(1)=="3920"){
                                        me.upd.availible_tan_verfahren=[];
                                        for(var a=3;a<HIRMS_for_tanv.store.data[i].data.length;a++){
                                            me.upd.availible_tan_verfahren.push(HIRMS_for_tanv.store.data[i].data[a]);
                                        }
                                        if(me.upd.availible_tan_verfahren.length>0)
                                            me.log.gv.info({gv:"HKVVB"},"Update to use Tan procedure: "+me.upd.availible_tan_verfahren[0]);
                                        break;
                                    }
                                }
                            }catch(ee){me.log.gv.error(ee,{gv:"HKVVB"},"Error while analyse HKVVB result Tan Verfahren");}
                            // 8. Analysiere Geschäftsvorfallparameter
                            try{
                                for(var i in recvMsg.segments){
                                    if(recvMsg.segments[i].name.length>=6&&recvMsg.segments[i].name.charAt(5)=="S"){
                                        var gv = recvMsg.segments[i].name.substring(0,5);
                                        if(!(gv in me.bpd.gv_parameters)){
                                            me.bpd.gv_parameters[gv]={};
                                        }
                                        me.bpd.gv_parameters[gv][recvMsg.segments[i].vers]=recvMsg.segments[i];
                                    }
                                }
                            }catch(ee){me.log.gv.error(ee,{gv:"HKVVB"},"Error while analyse HKVVB result Tan Verfahren");}
                            try{
                                cb(error,recvMsg,has_neu_url);
                            }catch(cb_error){
                                me.log.gv.error(cb_error,{gv:"HKVVB"},"Unhandled callback Error in HKVVB,HKIDN");
                            }
                        }catch(e){
                            me.log.gv.error(e,{gv:"HKVVB"},"Error while analyse HKVVB Response");
                            try{
                                cb(e.toString(),null,false);
                            }catch(cb_error){
                                me.log.gv.error(cb_error,{gv:"HKVVB"},"Unhandled callback Error in HKVVB,HKIDN");
                            }
                        }
                    }else{
                        me.log.gv.error({gv:"HKVVB"},"Error while analyse HKVVB Response No Init Successful recv.");
                        try{
                            cb("Keine Initialisierung Erfolgreich Nachricht erhalten!",recvMsg,false);
                        }catch(cb_error){
                            me.log.gv.error(cb_error,{gv:"HKVVB"},"Unhandled callback Error in HKVVB,HKIDN");
                        }
                    }
                }else{
                    me.log.gv.error({gv:"HKVVB",hirmsg:HIRMG},"Error while analyse HKVVB Response Wrong HIRMG response code");
                    try{
                        cb("Fehlerhafter Rückmeldungscode: "+(HIRMG===null?"keiner":HIRMG.getEl(1).getEl(3)),recvMsg,false);
                    }catch(cb_error){
                        me.log.gv.error(cb_error,{gv:"HKVVB"},"Unhandled callback Error in HKVVB,HKIDN");
                    }
                }
            }
        });
    };


    me.MsgCheckAndEndDialog = function(recvMsg,cb){
        var HIRMGs = recvMsg.selectSegByName("HIRMG");
        for(var k in HIRMGs){
            for(var i in (HIRMGs[k].store.data)){
                var ermsg = HIRMGs[k].store.data[i].getEl(1);
                if(ermsg=="9800"){
                    try{
                        cb(null,null);
                    }catch(cb_error){
                        me.log.gv.error(cb_error,{gv:"HKEND"},"Unhandled callback Error in HKEND");
                    }
                    return;
                }
            }
        }
        me.MsgEndDialog(cb);
    };
    me.MsgEndDialog = function(cb){
        var msg = new Nachricht(me.proto_version);
        if(me.kunden_id!=9999999999)msg.sign({'pin':me.pin,'tan':NULL,'sys_id':me.sys_id,'pin_vers':me.upd.availible_tan_verfahren[0],'sig_id':me.getNewSigId()});
        msg.init(me.dialog_id, me.next_msg_nr,me.blz,me.kunden_id);
        me.next_msg_nr++;
        msg.addSeg(Helper.newSegFromArray("HKEND", 1, [me.dialog_id]));
        me.SendMsgToDestination(msg,function(error,recvMsg){
            if(error){
                me.log.gv.error(error,{gv:"HKEND",msg:msg},"HKEND could not be send");
            }
            try{
                cb(error,recvMsg);
            }catch(cb_error){
                me.log.gv.error(cb_error,{gv:"HKEND"},"Unhandled callback Error in HKEND");
            }
        });
    };

    // SEPA kontoverbindung anfordern HKSPA, HISPA ist die antwort
    me.MsgRequestSepa = function(for_konto,cb){
        //Vars
        var processed = false;
        var v1=null;
        var aufsetzpunkt_loc = 0;
        var sepa_list=new Array();
        // Create Segment
        if(for_konto){
            v1 = [[280,for_konto]];
            aufsetzpunkt_loc = 2;
        }else{
            v1 = [];
            aufsetzpunkt_loc = 1;
        }
        // Start
        var req_sepa = new Order(me);
        req_sepa.msg({
            type:"HKSPA",
            ki_type:"HISPA",
            aufsetzpunkt_loc:[aufsetzpunkt_loc],
            send_msg:{
                1:v1,
                2:v1,
                3:v1
            },
            recv_msg:req_sepa.Helper().vers([1,2,3],function(seg_vers,relatedRespSegments,relatedRespMsgs,recvMsg){
                try{
                    if(req_sepa.checkMessagesOkay(relatedRespMsgs,true)){
                        var HISPA = req_sepa.getSegByName(relatedRespSegments,"HISPA");
                        if(HISPA!=null){
                            for(var i=0;i!=HISPA.store.data.length;i++){
                                var verb = HISPA.getEl(i+1);
                                var o = {};
                                o.is_sepa = verb.getEl(1)=="J";
                                o.iban    = verb.getEl(2);
                                o.bic     = verb.getEl(3);
                                o.konto_nr = verb.getEl(4);
                                o.unter_konto = verb.getEl(5);
                                o.ctry_code = verb.getEl(6);
                                o.blz        = verb.getEl(7);
                                sepa_list.push(o);
                            }
                            try{
                                cb(null,recvMsg,sepa_list);
                            }catch(cb_error){
                                me.log.gv.error(cb_error,{gv:"HKSPA"},"Unhandled callback Error in HKSPA");
                            }
                        }else{
                            throw new Error("TODO ausführlicherer Error");
                        }
                    }
                }catch(e){
                    me.log.gv.error(e,{gv:"HKSPA",msgs:relatedRespMsgs,segments:relatedRespSegments},"Exception while parsing HKSPA response");
                    try{
                        cb(e,null,null);
                    }catch(cb_error){
                        me.log.gv.error(cb_error,{gv:"HKSPA"},"Unhandled callback Error in HKSPA");
                    }
                }
                processed = true;
            }).done()
        });
        req_sepa.done(function(error,order,recvMsg){
            if(error&&!processed){
                me.log.gv.error(error,{gv:"HKSPA",recvMsg:recvMsg},"Exception while parsing HKSPA");
                try{
                    cb(error,recvMsg,null);
                }catch(cb_error){
                    me.log.gv.error(cb_error,{gv:"HKSPA"},"Unhandled callback Error in HKSPA");
                }
            }else if(!processed){
                error = new Exceptions.InternalError("HKSPA response was not analysied");
                me.log.gv.error(error,{gv:"HKSPA",recvMsg:recvMsg},"HKSPA response was not analysied");
                try{
                    cb(error,recvMsg,null);
                }catch(cb_error){
                    me.log.gv.error(cb_error,{gv:"HKSPA"},"Unhandled callback Error in HKSPA");
                }
            }

        });
    };

    /*
     konto = {iban,bic,konto_nr,unter_konto,ctry_code,blz}
     from_date
     to_date		können null sein
     cb
     */
    me.MsgGetKontoUmsaetze = function(konto,from_date,to_date,cb){
        var processed = false;
        var v7=null;
        var v5=null;
        if(from_date==null&&to_date==null){
            v5 = [[konto.konto_nr,konto.unter_konto,konto.ctry_code,konto.blz],"N"];
            v7 = [[konto.iban,konto.bic,konto.konto_nr,konto.unter_konto,konto.ctry_code,konto.blz],"N"];
        }else{
            v5 = [[konto.konto_nr,konto.unter_konto,konto.ctry_code,konto.blz],"N",Helper.convertDateToDFormat(from_date),Helper.convertDateToDFormat(to_date)];
            v7 = [[konto.iban,konto.bic,konto.konto_nr,konto.unter_konto,konto.ctry_code,konto.blz],"N",Helper.convertDateToDFormat(from_date),Helper.convertDateToDFormat(to_date)];
        }
        // Start
        var req_umsaetze = new Order(me);
        var recv = function(seg_vers,relatedRespSegments,relatedRespMsgs,recvMsg){
            try{
                if(req_umsaetze.checkMessagesOkay(relatedRespMsgs,true)){
                    // Erfolgreich Meldung
                    var txt = "";
                    for(var i in relatedRespSegments){
                        if(relatedRespSegments[i].name=="HIKAZ"){
                            var HIKAZ = relatedRespSegments[i];
                            txt += HIKAZ.getEl(1);
                        }
                    }
                    var mtparse = new MTParser();
                    mtparse.parse(txt);
                    var umsatze = mtparse.getKontoUmsaetzeFromMT490();
                    // Callback
                    try{
                        cb(null,recvMsg,umsatze);
                    }catch(cb_error){
                        me.log.gv.error(cb_error,{gv:"HKKAZ"},"Unhandled callback Error in HKKAZ");
                    }
                }
            }catch(ee){
                me.log.gv.error(ee,{gv:"HKKAZ",resp_msg:recvMsg},"Exception while parsing HKKAZ response");
                // Callback
                try{
                    cb(ee,recvMsg,null);
                }catch(cb_error){
                    me.log.gv.error(cb_error,{gv:"HKKAZ"},"Unhandled callback Error in HKKAZ");
                }
            }
            processed = true;
        };
        // TODO check if we can do the old or the new version HKCAZ
        req_umsaetze.msg({
            type:"HKKAZ",
            ki_type:"HIKAZ",
            aufsetzpunkt_loc:[6],
            send_msg:{
                7:v7,
                5:v5
            },
            recv_msg:{
                7:recv,
                5:recv
            }
        });
        req_umsaetze.done(function(error,order,recvMsg){
            if(error&&!processed){
                me.log.gv.error(error,{gv:"HKKAZ",recvMsg:recvMsg},"HKKAZ could not be send");
                // Callback
                try{
                    cb(error,recvMsg,null);
                }catch(cb_error){
                    me.log.gv.error(cb_error,{gv:"HKKAZ"},"Unhandled callback Error in HKKAZ");
                }
            }else if(!processed){
                error = new Exceptions.InternalError("HKKAZ response was not analysied");
                me.log.gv.error(error,{gv:"HKKAZ",recvMsg:recvMsg},"HKKAZ response was not analysied");
                // Callback
                try{
                    cb(error,recvMsg,null);
                }catch(cb_error){
                    me.log.gv.error(cb_error,{gv:"HKKAZ"},"Unhandled callback Error in HKKAZ");
                }
            }
        });
    };

    /*
     konto = {iban,bic,konto_nr,unter_konto,ctry_code,blz}
     cb
     */
    me.MsgGetSaldo = function(konto,cb){
        var req_saldo = new Order(me);
        var processed = false;
        var v5 = null;
        var v7 = null;
        var avail_send_msg = {};
        if('iban' in konto&&'bic' in konto&&req_saldo.checkKITypeAvailible("HISAL",[7])){
            var konto_verb_int = [konto.iban,konto.bic,konto.konto_nr,konto.unter_konto,konto.ctry_code,konto.blz];
            v7 = [konto_verb_int,"N"];
            avail_send_msg[7]=v7;
        }else{
            var konto_verb = [konto.konto_nr,konto.unter_konto,konto.ctry_code,konto.blz];
            v5 = [konto_verb,"N"];
            avail_send_msg[5]=v5;
            avail_send_msg[6]=v5;
        }
        // Start
        req_saldo.msg({
            type:"HKSAL",
            ki_type:"HISAL",
            send_msg:avail_send_msg,
            recv_msg:req_saldo.Helper().vers([5,6,7],function(seg_vers,relatedRespSegments,relatedRespMsgs,recvMsg){
                try{
                    if(req_saldo.checkMessagesOkay(relatedRespMsgs,true)){
                        var HISAL = req_saldo.getSegByName(relatedRespSegments,"HISAL");
                        if(HISAL!=null){
                            try{
                                var result ={
                                    desc:				req_saldo.getElFromSeg(HISAL,2,null),
                                    cur:				req_saldo.getElFromSeg(HISAL,3,null),
                                    saldo:				Helper.getSaldo(HISAL,4,false),
                                    saldo_vorgemerkt:	Helper.getSaldo(HISAL,5,false),
                                    credit_line:		Helper.getBetrag(HISAL,6),
                                    avail_amount:		Helper.getBetrag(HISAL,7),
                                    used_amount:		Helper.getBetrag(HISAL,8),
                                    overdraft:			null,
                                    booking_date:		null,
                                    faelligkeit_date:	Helper.getJSDateFromSeg(HISAL,11)
                                };
                                if(seg_vers==5){
                                    result.booking_date = Helper.getJSDateFromSeg(HISAL,9,10);
                                }else{
                                    result.booking_date = Helper.getJSDateFromSegTSP(HISAL,11);
                                    result.overdraft    = Helper.getBetrag(HISAL,9);
                                }
                                cb(null,recvMsg,result);
                            }catch(cb_error){
                                me.log.gv.error(cb_error,{gv:"HKSAL"},"Unhandeled callback Error in HKSAL");
                            }
                        }else{
                            throw new Error("TODO ausführlicherer Error");
                        }
                    }
                }catch(e){
                    me.log.gv.error(e,{gv:"HKSAL",msgs:relatedRespMsgs,segments:relatedRespSegments},"Exception while parsing HKSAL response");
                    try{
                        cb(e,null,null);
                    }catch(cb_error){
                        me.log.gv.error(cb_error,{gv:"HKSAL"},"Unhandeled callback Error in HKSAL");
                    }
                }
                processed = true;
            }).done()
        });
        req_saldo.done(function(error,order,recvMsg){
            if(error&&!processed){
                me.log.gv.error(error,{gv:"HKSAL",recvMsg:recvMsg},"Exception while parsing HKSAL");
                try{
                    cb(error,recvMsg,null);
                }catch(cb_error){
                    me.log.gv.error(cb_error,{gv:"HKSAL"},"Unhandeled callback Error in HKSAL");
                }
            }else if(!processed){
                error = new Exceptions.InternalError("HKSAL response was not analysed");
                me.log.gv.error(error,{gv:"HKSAL",recvMsg:recvMsg},"HKSAL response was not analysed");
                try{
                    cb(error,recvMsg,null);
                }catch(cb_error){
                    me.log.gv.error(cb_error,{gv:"HKSAL"},"Unhandled callback Error in HKSAL");
                }
            }

        });
    };
    me.EstablishConnection = function(cb){
        var protocol_switch = false;
        var vers_step    = 1;
        var original_bpd = me.bpd.clone();original_bpd.clone = me.bpd.clone;
        var original_upd = me.upd.clone();original_upd.clone = me.upd.clone;
        // 1. Normale Verbindung herstellen um BPD zu bekommen und evtl. wechselnde URL ( 1.versVersuch FinTS 2. versVersuch HBCI2.2 )
        // 2. Verbindung mit richtiger URL um auf jeden Fall (auch bei geänderter URL) die richtigen BPD zu laden + Tan Verfahren herauszufinden
        // 3. Abschließende Verbindung aufbauen
        var perform_step = function(step){
            me.MsgInitDialog(function(error,recvMsg,has_neu_url){
                if(error){
                    me.MsgCheckAndEndDialog(recvMsg,function(error2,recvMsg2){if(error2){me.log.conest.error({step:step,error:error2},"Connection close failed.");}else{me.log.conest.debug({step:step},"Connection closed okay.");}});
                    if(vers_step==1){
                        try{
                            var HIRMS = recvMsg.selectSegByNameAndBelongTo("HIRMS",1)[0];
                            if(HIRMS.getEl(1).getEl(1)=="9120"){
                                me.log.conest.debug({step:step,hirms:HIRMS},"Test Switch Version from FinTS to HBCI2.2");
                                me.proto_version = 220;
                                vers_step = 2;
                                protocol_switch = true;
                                me.clear();
                                perform_step(1);
                            }
                        }catch(e){
                            me.log.conest.error(e,{step:step,error2:error},"Init Dialog failed and no switch version: "+error);
                        }
                    }else if(protocol_switch){
                        me.log.conest.error({step:step,hirms:HIRMS,error:error},"Test Switch Version from FinTS to HBCI2.2 failed.E:"+error);
                        me.proto_version = 300;
                        // Callback
                        try{
                            cb(error);
                        }catch(cb_error){
                            me.log.conest.error(cb_error,{step:step},"Unhandled callback Error in EstablishConnection");
                        }
                    }else{
                        me.log.conest.error({step:step,error:error},"Init Dialog failed: "+error);
                        // Callback
                        try{
                            cb(error);
                        }catch(cb_error){
                            me.log.conest.error(cb_error,{step:step},"Unhandled callback Error in EstablishConnection");
                        }
                    }
                }else{
                    // Erfolgreich Init Msg verschickt
                    me.log.conest.debug({step:step,bpd:beautifyBPD(me.bpd),upd:me.upd,url:me.bpd.url,new_sig_method:me.upd.availible_tan_verfahren[0]},"Init Dialog successful.");
                    if(step==1||step==2){
                        // Im Step 1 und 2 bleiben keine Verbindungen erhalten
                        // Diese Verbindung auf jeden Fall beenden
                        var neu_url = me.bpd.url;
                        var neu_sig_method = me.upd.availible_tan_verfahren[0];
                        me.bpd = original_bpd.clone();
                        me.upd = original_upd.clone();
                        var orig_sys_id   = me.sys_id;
                        var orig_last_sig = me.last_signatur_id;
                        me.MsgCheckAndEndDialog(recvMsg,function(error2,recvMsg2){if(error2){me.log.conest.error({step:step,error:error2},"Connection close failed.");}else{me.log.conest.debug({step:step},"Connection closed okay.");}});
                        me.clear();
                        me.bpd.url 						  = neu_url;
                        me.upd.availible_tan_verfahren[0] = neu_sig_method;
                        me.sys_id 						  = orig_sys_id;
                        me.last_signatur_id 			  = orig_last_sig;
                        original_bpd.url 						= me.bpd.url;
                        original_upd.availible_tan_verfahren[0] = neu_sig_method;
                    }

                    if(has_neu_url){
                        if(step==1){
                            // Im Step 1 ist das eingeplant, dass sich die URL ändert
                            me.log.conest.debug({step:2},"Start Connection in Step 2");
                            perform_step(2);
                        }else{
                            // Wir unterstützen keine mehrfach Ändernden URLs
                            if(step==3){
                                me.bpd = original_bpd.clone();
                                me.upd = original_upd.clone();
                                me.MsgCheckAndEndDialog(recvMsg,function(error2,recvMsg2){if(error2){me.log.conest.error({step:step,error:error2},"Connection close failed.");}else{me.log.conest.debug({step:step},"Connection closed okay.");}});
                            }
                            me.log.conest.error({step:step},"Multiple URL changes are not supported!");
                            // Callback
                            try{
                                cb("Mehrfachänderung der URL ist nicht unterstützt!");
                            }catch(cb_error){
                                me.log.conest.error(cb_error,{step:step},"Unhandled callback Error in EstablishConnection");
                            }
                        }
                    }else if(step==1||step==2){
                        // 3: eigentliche Verbindung aufbauen
                        me.log.conest.debug({step:3},"Start Connection in Step 3");
                        perform_step(3);
                    }else{
                        // Ende Schritt 3 = Verbindung Ready
                        me.log.conest.debug({step:step},"Connection entirely established. Now get the available accounts.");
                        // 4. Bekomme noch mehr Details zu den Konten über HKSPA
                        me.MsgRequestSepa(null,function(error,recvMsg2,sepa_list){
                            if(error){
                                me.log.conest.error({step:step},"Error getting the available accounts.");
                                me.MsgCheckAndEndDialog(recvMsg,function(error3,recvMsg2){if(error3){me.log.conest.error({step:step,error:error2},"Connection close failed.");}else{me.log.conest.debug({step:step},"Connection closed okay.");}});
                                // Callback
                                try{
                                    cb(error);
                                }catch(cb_error){
                                    me.log.conest.error(cb_error,{step:step},"Unhandled callback Error in EstablishConnection");
                                }
                            }else{
                                // Erfolgreich die Kontendaten geladen, diese jetzt noch in konto mergen und Fertig!
                                for(var i=0;i!=sepa_list.length;i++){
                                    for(var j=0;j!=me.konten.length;j++){
                                        if(me.konten[j].konto_nr	== sepa_list[i].konto_nr&&
                                            me.konten[j].unter_konto == sepa_list[i].unter_konto ){
                                            me.konten[j].sepa_data = sepa_list[i];
                                            break;
                                        }
                                    }
                                }
                                // Fertig
                                me.log.conest.debug({step:step,recv_sepa_list:sepa_list},"Connection entirely established and got available accounts. Return.");
                                // Callback
                                try{
                                    cb(null);
                                }catch(cb_error){
                                    me.log.conest.error(cb_error,{step:step},"Unhandled callback Error in EstablishConnection");
                                }
                            }
                        });
                    }
                }
            });
        };
        me.log.conest.debug({step:1},"Start First Connection");
        perform_step(1);
    };

    //
    me.SendMsgToDestination = function(msg,callback){// Parameter für den Callback sind error,data
        var txt = msg.transformForSend();
        me.debugLogMsg(txt,true);
        networkRequest(txt, me.bpd.url, function(error, clear_txt){

            if (error !== null){
                callback(new Exceptions.ConnectionFailedException(error.hostname, error.port, error.path), null);
                return;
            }

            try {
                var MsgRecv = new Nachricht(me.proto_version);
                MsgRecv.parse(clear_txt);
                callback(null, MsgRecv);
            } catch (e) {
                callback(e.toString(), null);
            }
        });
    };

    me.debugLogMsg = function(txt,send){
        me.log.con.trace({raw_data:txt,send_or_recv:send?"send":"recv"},"Connection Data Trace");
        if(me.debug_mode)
            Console.log((send?"Send: ":"Recv: ")+txt);
    };

};

var url = getParameter('url');
var blz = getParameter('blz');
var user_id = getParameter('user_id');
var pin = getParameter('pin');

// 1. Definition der Bankenliste - Echte URLs sind hier http://www.hbci-zka.de/institute/institut_auswahl.htm erhältlich
var bankenliste = {};
bankenliste[blz] = {'blz':blz,'url':url};
// 2. FinTSClient anlegen
// BLZ: 70051540
// Kunden-ID/Benutzerkennung: test1
// PIN: 1234
// Bankenliste siehe oben
var client = new FinTSClient(blz,user_id,pin,bankenliste);
// 3. Verbindung aufbauen
client.EstablishConnection(function(error){
			if(error){
				Console.log("Fehler: "+error);
			}else{
				Console.log("Erfolgreich Verbunden");
				// 4. Kontoumsätze für das 1. Konto(client.konten[0]) laden
				client.MsgGetKontoUmsaetze(client.konten[0].sepa_data,null,null,function(error2,rMsg,data){
					if(error){
						Console.log("Fehler beim laden der Umsätze: "+error2);
					}else{
                        for(var d in data) {
                            var saetze = data[d].saetze;
                            for (var i in saetze) {
                                var saetz = saetze[i];
                                var desc = saetz.verwendungszweck;
                                var amount = saetz.value;
                                if (saetz.soll_haben == "S")
                                  amount *= -1;
                                var date ={
                                    y : saetz.datum.getFullYear(),
                                    m : saetz.datum.getMonth()+1,
                                    d : saetz.datum.getDate()
                                }
                                addStatement(amount, desc.buchungstext, date, desc.name_kontrahent, desc.iban_kontrahent, desc.text);
                            }
                        }
						// 5. Verbindung beenden
						client.MsgEndDialog(function(error,recvMsg2){
							// 6. Secure Daten im Objekt aus dem Ram löschen
							client.closeSecure();
							Console.log("ENDE");
						});
					}
				});
			}
		});
