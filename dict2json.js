/*
    dict2json.js

    the function in this file is called by my max patcher. it takes input from a max dict object, and outputs
    a formatted JSON string that my JUCE app can easily process into a storable dictionary.
*/

function anything() {
    var d = new Dict("newDevice");
    var jsonString = d.stringify();
    outlet(0, jsonString);
}