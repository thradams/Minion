
//Runtime
/**
 * xmlhttp.open("POST", "/json-handler");
   xmlhttp.setRequestHeader("Content-Type", "application/json;charset=UTF-8");
   xmlhttp.send(JSON.stringify({ "email": "hello@user.com", "response": { "name": "Tester" } }));
 */

function HttpRequest(url, method, onRequestCompleted, onRequestFailed, data, timeoutMs) 
{
    try
    {
        //url = "\func\1";
        var xhr = new XMLHttpRequest();
        xhr.open(method, url);
        xhr.timeout = (timeoutMs) ? timeoutMs : 5000;
        var token = "token";
        if (token)
        {
            /*
            Used to prevent cross-site request forgery. Alternative header names are: X-CSRFToken[27] and X-XSRF-TOKEN[28]
            https://en.wikipedia.org/wiki/List_of_HTTP_header_fields#Common_non-standard_request_fields
            case 19157
            */
            xhr.setRequestHeader("X-Csrf-Token", token);
        }
        else
        {
            xhr.setRequestHeader("X-Csrf-Token", "0");
        }
        xhr.onreadystatechange = function ()
        {
            try
            {

                switch (xhr.readyState)
                {
                    case 0:
                        break;
                    case 1:
                        break;
                    case 2:
                        break;
                    case 3:
                        break;
                    case 4:
                        var xhr_status = xhr.status;
                        /*var responseJson = {
                            Message: IDS("SERVER_IS_NOT_RESPONDING") + " " + xhr.statusText,
                            Code: ServerResponseCode.Fail
                        };*/
                        //if ((typeof navigator.onLine != "undefined") &&
                        //  !navigator.onLine) {
                        //                            responseJson =
                        //                              {
                        //                                Message: "Your Browser seems to be Offline.",
                        //                              Code: ServerResponseCode.Fail
                        //                        };
                        //}
                        if (xhr_status != 0 &&
                            xhr_status != 304)
                        {
                            try
                            {
                                if (xhr.responseText)
                                {
                                    responseJson = JSON.parse(xhr.responseText);
                                }
                                else
                                {
                                    responseJson = {};
                                }
                            }
                            catch (er)
                            {
                                if (xhr_status == 200)
                                {
                                    xhr_status = 0;
                                }
                                //responseJson =
                                //  {
                                //    Message: "Invalid server JSON response.",
                                //  Code: ServerResponseCode.Fail
                                //};
                            }
                        }
                        switch (xhr_status)
                        {
                            case 0:
                                onRequestFailed(responseJson);
                                break;
                            case 200:
                                onRequestCompleted(responseJson);
                                break;
                            case 204:
                                onRequestCompleted({});
                                break;
                            case 304:
                                //responseJson.Code = ServerResponseCode.NotModified;
                                onRequestFailed(responseJson);
                                break;
                            case 401:
                                //if (responseJson.Code == ServerResponseCode.Disconnected ||
                                //  responseJson.Code == ServerResponseCode.InvalidToken) {
                                //this.OnDisconnected(responseJson);
                                //onRequestFailed(responseJson);
                                //}
                                //else {
                                //  onRequestFailed(responseJson);
                                //}
                                break;
                            default:
                                //onRequestFailed(responseJson);
                                break;
                        }
                        break;
                    default:
                        console.error("Invalid XMLHttpRequest readyState: " + xhr.readyState);
                }
            }
            catch (e)
            {
                onRequestFailed(e);
            }
        };
        xhr.setRequestHeader('Content-type', 'application/x-www-form-urlencoded');
        xhr.send(data);
    }
    catch (e)
    {
        onRequestFailed(e);
    }
};

function HttpRequest2(data, onRequestCompleted, onRequestFailed, timeoutMs) 
{
    try
    {
        //url = ;
        var xhr = new XMLHttpRequest();
        xhr.open("POST", "\\call");

        xhr.timeout = (timeoutMs) ? timeoutMs : 5000;
        var token = "token";
        if (token)
        {
            /*
            Used to prevent cross-site request forgery. Alternative header names are: X-CSRFToken[27] and X-XSRF-TOKEN[28]
            https://en.wikipedia.org/wiki/List_of_HTTP_header_fields#Common_non-standard_request_fields
            case 19157
            */
            xhr.setRequestHeader("X-Csrf-Token", token);
        }
        else
        {
            xhr.setRequestHeader("X-Csrf-Token", "0");
        }
        xhr.onreadystatechange = function ()
        {
            try
            {

                switch (xhr.readyState)
                {
                    case 0:
                        break;
                    case 1:
                        break;
                    case 2:
                        break;
                    case 3:
                        break;
                    case 4:
                        var xhr_status = xhr.status;
                        /*var responseJson = {
                            Message: IDS("SERVER_IS_NOT_RESPONDING") + " " + xhr.statusText,
                            Code: ServerResponseCode.Fail
                        };*/
                        //if ((typeof navigator.onLine != "undefined") &&
                        //  !navigator.onLine) {
                        //                            responseJson =
                        //                              {
                        //                                Message: "Your Browser seems to be Offline.",
                        //                              Code: ServerResponseCode.Fail
                        //                        };
                        //}
                        if (xhr_status != 0 &&
                            xhr_status != 304)
                        {
                            try
                            {
                                if (xhr.responseText)
                                {
                                    responseJson = JSON.parse(xhr.responseText);
                                }
                                else
                                {
                                    responseJson = {};
                                }
                            }
                            catch (er)
                            {
                                if (xhr_status == 200)
                                {
                                    xhr_status = 0;
                                }
                                //responseJson =
                                //  {
                                //    Message: "Invalid server JSON response.",
                                //  Code: ServerResponseCode.Fail
                                //};
                            }
                        }
                        switch (xhr_status)
                        {
                            case 0:
                                onRequestFailed(responseJson);
                                break;
                            case 200:
                                onRequestCompleted(responseJson);
                                break;
                            case 204:
                                onRequestCompleted({});
                                break;
                            case 304:
                                //responseJson.Code = ServerResponseCode.NotModified;
                                onRequestFailed(responseJson);
                                break;
                            case 401:
                                //if (responseJson.Code == ServerResponseCode.Disconnected ||
                                //  responseJson.Code == ServerResponseCode.InvalidToken) {
                                //this.OnDisconnected(responseJson);
                                //onRequestFailed(responseJson);
                                //}
                                //else {
                                //  onRequestFailed(responseJson);
                                //}
                                break;
                            default:
                                //onRequestFailed(responseJson);
                                break;
                        }
                        break;
                    default:
                        console.error("Invalid XMLHttpRequest readyState: " + xhr.readyState);
                }
            }
            catch (e)
            {
                onRequestFailed(e);
            }
        };
        xhr.setRequestHeader("Content-Type", "application/json;charset=UTF-8");
        xhr.send(data);

    }
    catch (e)
    {
        onRequestFailed(e);
    }
};

function Call()
{
    var s = "[";
    s += JSON.stringify(arguments[0]);
    for (var i = 1; i < arguments.length - 1; i++)
    {
        s += ",";
        s += JSON.stringify(arguments[i]);
    }
    s += "]";

    var callback = arguments[arguments.length - 1];
    HttpRequest2(s, function (json)
    {
        callback(null, json);
    },
        function (error)
        {
            callback(error, null);
        });
}

