
function SWA_loadLearnedScenes(device, online, progress, context) {
    var parPersonName = device.getParameterByName("FINACT_PersonName");
    var parPersonFinger = device.getParameterByName("FINACT_PersonFinger");
    var parFingerId = device.getParameterByName("FINACT_FingerId");
    var parNumberSearchResults = device.getParameterByName("FINACT_NumberSearchResults");
    var parNumberSearchResultsText = device.getParameterByName("FINACT_NumberSearchResultsText");
    var parNumberSearchResultsToDisplay = device.getParameterByName("FINACT_NumberSearchResultsToDisplay");

    parNumberSearchResults.value = 0;
    parNumberSearchResultsToDisplay.value = 0;

    progress.setText("Fingerprint: Finger ID zu Person " + parPersonName.value + " (" + parPersonFinger.value + ") suchen...");
    online.connect();

    var data = [12]; // internal function ID

    // person finger
    data = data.concat((parPersonFinger.value & 0x000000ff));

    // person name
    for (var i = 0; i < parPersonName.value.length; ++i) {
        var code = parPersonName.value.charCodeAt(i);
        data = data.concat([code]);
    }
    data = data.concat(0); // null-terminated string

    var resp = online.invokeFunctionProperty(160, 3, data);
    if (resp[0] != 0) {
        if (resp[0] == 1) {
            progress.setText("Fingerprint: Finger ID zu Person " + parPersonName.value + " (" + parPersonFinger.value + ") nicht gefunden.");
            online.disconnect();
            return;
        } else {
            throw new Error("Fingerprint: Es ist ein unbekannter Fehler aufgetreten!");
        }
    }

    online.disconnect();

    var numRes = (resp.length - 3) / 31;
    var totalMatches = resp[1] << 8 | resp[2];
    // info("totalMatches " + totalMatches);
    progress.setText("Fingerprint: " + totalMatches + " Finger ID(s) zu Person " + parPersonName.value + " (" + parPersonFinger.value + ") gefunden.");

    // var fingerId = resp[1] << 8 | resp[2];
    // var personFinger = resp[3];
    // var personName = "";
    // for (var i = 4; i < 32; ++i) {
    //     if (resp[i] == 0)
    //         break; // null-termination

    //     personName += String.fromCharCode(resp[i]);
    // }

    // parPersonName.value = personName;
    // parPersonFinger.value = personFinger;
    // parFingerId.value = fingerId;


    // following up to 10 results in total
    // always 2 bytes fingerId, 1 byte personFinger and 28 bytes personName
    // info("Bevor: " + parNumberSearchResults.value);
    parNumberSearchResultsText.value = totalMatches;
    parNumberSearchResultsToDisplay.value = numRes;
    if (totalMatches > numRes) {
        parNumberSearchResults.value = 1;
    }

    // info("Danach: " + parNumberSearchResults.value);
    for (var row = 1; row <= numRes; row++) {
        // info("FINACT_Person" + row + "Name");
        parPersonName = device.getParameterByName("FINACTSER_Person" + row + "Name");
        parPersonFinger = device.getParameterByName("FINACTSER_Person" + row + "Finger");
        parFingerId = device.getParameterByName("FINACTSER_Finger" + row + "Id");

        var res = (row - 1) * 31 + 3;
        // info("res " + row + ": " + res);
        var fingerId = resp[res + 0] << 8 | resp[res + 1];
        // info("fingerid: " + fingerId);
        var personFinger = resp[res + 2];
        // info("finger: " + personFinger);
        var personName = "";
        for (var i = res + 3; i < res + 31; ++i) {
            if (resp[i] == 0)
                break; // null-termination

            personName += String.fromCharCode(resp[i]);
        }
        // info("personName: " + personName);

        parPersonName.value = personName;
        parPersonFinger.value = personFinger;
        parFingerId.value = fingerId;
    }
}