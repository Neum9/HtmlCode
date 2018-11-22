/**
 *  Neum
 *  
 *  2018 11 20
 * 
 *  binary util to solve the bit problem
 */

'use strict';

/**
 * 
 * @param {string} binStr
 * a string represent binary
 * @return {string}
 * a char represent
 */

function ConvertBinToChars(binStr) {
    var chars = "";

    //fill 0 back
    for (var i = 0, len = (8 - binStr.length % 8) % 8; i < len; i++) {
        binStr += "0";
    }

    for (var i = 0, len = binStr.length / 8; i < len; i++) {
        chars += String.fromCharCode(parseInt(binStr.substr(i * 8, 8), 2));
    }

    return chars;
}
/**
 * 
 * @param {number} dec 
 * @return {string}
 * a char represent
 */

function ConvertDecToChar(dec) {
    var charString = "";
    var binStringList = dec.toString(2);
    //fill 0 front
    for (var i = 0, len = (8 - binStringList.length % 8) % 8; i < len; i++) {
        binStringList = "0" + binStringList;
    }

    for (var i = 0; i < binStringList.length / 8; i++) {
        var byte = parseInt(binStringList.substr(i * 8, 8), 2);
        if (byte < 0 || byte > 255) {
            alert("Something wrong with the bin transform in ConvertDecToChar: byte " + byte);
        }
        charString += String.fromCharCode(byte);
    }

    return charString;
}

/**
 * 
 * @param {number} dec 
 * @return
 * 32bit(4 char)
 */

function ConvertDecTo4Char(dec) {
    if (dec < 0 || dec > Math.pow(2, 32)) {
        alert("Something wrong with the bin transform in ConvertDecTo4Char: dec " + dec);
    }

    var s = ConvertDecToChar(dec);

    //fill 0
    for (var i = 0, len = 4 - s.length; i < len; i++) {
        s = String.fromCharCode(0) + s;
    }

    return s;
}

function ConvertCharsToDec(chars) {

    var dec = 0;

    for (var i = 0, len = chars.length; i < len; i++) {
        dec += chars[i].charCodeAt();
    }

    return dec;
}

function ConvertCharsToBinString(chars) {

    var binStr = "";

    for (var i = 0, len = chars.length; i < len; i++) {
        var binTemp = chars[i].charCodeAt().toString(2);

        //fill 0 front
        for (var i = 0, len = (8 - binTemp.length % 8) % 8; i < len; i++) {
            binTemp = "0" + binTemp;
        }


        binStr += binTemp
    }

    return binStr;
}