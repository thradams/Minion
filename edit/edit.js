
function HttpRequest(url, method, onRequestCompleted, onRequestFailed, data, timeoutMs) 
{
    try {
        //url = "\func\1";
        var xhr = new XMLHttpRequest();
        xhr.open(method, url);
        xhr.timeout = (timeoutMs) ? timeoutMs : 5000;
        var token = "token";
        if (token) {
            /*
            Used to prevent cross-site request forgery. Alternative header names are: X-CSRFToken[27] and X-XSRF-TOKEN[28]
            https://en.wikipedia.org/wiki/List_of_HTTP_header_fields#Common_non-standard_request_fields
            case 19157
            */
            xhr.setRequestHeader("X-Csrf-Token", token);
        }
        else {
            xhr.setRequestHeader("X-Csrf-Token", "0");
        }
        xhr.onreadystatechange = function () {
            try {
                
                switch (xhr.readyState) {
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
                            xhr_status != 304) {
                            try {
                                responseJson = JSON.parse(xhr.responseText);
                            }
                            catch (er) {
                                if (xhr_status = 200) {
                                    xhr_status = 0;
                                }
                                //responseJson =
                                  //  {
                                    //    Message: "Invalid server JSON response.",
                                      //  Code: ServerResponseCode.Fail
                                    //};
                            }
                        }
                        switch (xhr_status) {
                            case 0:
                                onRequestFailed(responseJson);
                                break;
                            case 200:
                                onRequestCompleted(responseJson);
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
            catch (e) {
                onRequestFailed(e);
            }
        };
        xhr.setRequestHeader('Content-type', 'application/x-www-form-urlencoded');
        xhr.send(data);
    }
    catch (e) {
        onRequestFailed(e);
    }
};

function BuildApp()
{
    HttpRequest("func/1", 'GET', function(json){

    },
    function (e){

    });

    //alert('build');
}

function getQueryStringValue(key)
{
  return decodeURIComponent(window.location.search.replace(new RegExp("^(?:.*[&\\?]" + encodeURIComponent(key).replace(/[\.\+\*]/g, "\\$&") + "(?:\\=([^&]*))?)?.*$", "i"), "$1"));
} 

function SendText(fileName, fileContentTxt, onCompleted)
{
  var xmlhttp = new XMLHttpRequest();

  xmlhttp.upload.addEventListener("progress", onCompleted);
  xmlhttp.upload.addEventListener("load", onCompleted);
  xmlhttp.upload.addEventListener("error", onCompleted);
  xmlhttp.upload.addEventListener("abort", onCompleted);

  xmlhttp.open("POST", fileName);
  
  xmlhttp.onreadystatechange = function ()
  {
    if (xmlhttp.readyState == XMLHttpRequest.DONE && xmlhttp.status == 200)
    {
       // Request finished. Do processing here.
      //onCompleted(null);
    }
  };
  xmlhttp.setRequestHeader('Content-Type', 'text/xml');
  xmlhttp.send(fileContentTxt);
}

var myCodeMirrorXML;
var myCodeMirrorJS;

function SaveFiles()
{
  var name = getQueryStringValue("name");
  SendText(name + ".xml", myCodeMirrorXML.getValue(), function ()
  {
    SendText(name + ".js", myCodeMirrorJS.getValue(), function ()
    {
      BuildApp();
    });
  });
  
}



function OnPageLoad()
{
  myCodeMirrorXML = CodeMirror.fromTextArea(document.getElementById('xmltext'),
    {
      lineNumbers: true,
      matchBrackets: true,
      mode: "text/xml"
    });

  myCodeMirrorXML.setSize(null, 500);

  myCodeMirrorJS = CodeMirror.fromTextArea(document.getElementById('jstext'),
    {
      lineNumbers: true,
      matchBrackets: true,
      mode: "text/javascript"
    });
  myCodeMirrorJS.setSize(null, 500);

  var name = getQueryStringValue("name");
  
   var xhr = new XMLHttpRequest();
    xhr.open('GET', "/source/" + name + '.xml', true);
    xhr.onload = function ()
    {
      if (xhr.readyState === xhr.DONE)
      {
        if (xhr.status === 200)
        {
          myCodeMirrorXML.setValue(xhr.response);
          //document.getElementById("xmltext").innerText = xhr.response; 
          //localxml[name] = xhr.response;
          //var e = CreateClass(name, true, null);
          //document.body.innerText = "";
          //document.body.appendChild(e.domElem);
        }
      }
    };
  xhr.send(null);

  //var name = "/source/Tela1";
  var xhr2 = new XMLHttpRequest();
  xhr2.open('GET', "/source/" + name + '.js', true);
  xhr2.onload = function ()
  {
    if (xhr2.readyState === xhr.DONE)
    {
      if (xhr2.status === 200)
      {
        //document.getElementById("jstext").innerText = xhr2.response;
        myCodeMirrorJS.setValue(xhr2.response);
      }
    }
  };
  xhr2.send(null);

}