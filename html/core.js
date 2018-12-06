var Design = {};

function Create(node, screen, parent, parentElem)
{
    var child_n = node.firstChild;
    while (child_n)
    {
        if (child_n.nodeType == 3) /*#text*/
        {
            var txtNode = document.createTextNode(child_n.data);
            parentElem.appendChild(txtNode);
        } else
        {
            var className = child_n.nodeName;
            if (Design[className])
            {
                var parser = new DOMParser();
                var doc = parser.parseFromString(Design[className], "application/xml");
                var nodeClass = doc.firstChild;
                //se nada for dito o controle eh sempre div
                var elemType = 'div';
                if (nodeClass.attributes["Class"])
                {
                    //se tiver o atributo class eh tipo
                    elemType = nodeClass.attributes["Class"].nodeValue;
                }
                var childObject = eval("new " + className + "();");
                //seta a propriedade tela do item
                childObject.Screen = screen;
                //seta o elemento dom
                childObject.htmlElement = document.createElement(elemType);
            

                Create(nodeClass, screen, childObject,
                    childObject.htmlElement);
                //seta os atributos da classe
                for (var j = 0; j < nodeClass.attributes.length; j++)
                {


                    if (nodeClass.attributes[j].nodeName in childObject)
                    {
                        console.log("childObject[" + nodeClass.attributes[j].nodeName + "] = " + nodeClass.attributes[j].nodeValue);
                        childObject[nodeClass.attributes[j].nodeName] =
                            nodeClass.attributes[j].nodeValue;
                    }
                    else if (nodeClass.attributes[j].nodeName in childObject.htmlElement)
                    {
                        console.log("childObject.htmlElement[" + nodeClass.attributes[j].nodeName + "] = " + nodeClass.attributes[j].nodeValue);
                        childObject.htmlElement[nodeClass.attributes[j].nodeName] =
                            nodeClass.attributes[j].nodeValue;
                    }
                    else
                    {
                        console.log("Key [" + nodeClass.attributes[j].nodeName + "] NOT FOUND");
                    }

                }
                //seta as propriedades da instancia
                for (var j = 0; j < child_n.attributes.length; j++)
                {

                    if (child_n.attributes[j].nodeName in childObject)
                    {
                        console.log("childObject[" + child_n.attributes[j].nodeName + "] = " + child_n.attributes[j].nodeValue);
                        childObject[child_n.attributes[j].nodeName] =
                            child_n.attributes[j].nodeValue;
                    }
                    else if (child_n.attributes[j].nodeName in childObject.htmlElement)
                    {
                        console.log("childObject.htmlElement[" + child_n.attributes[j].nodeName + "] = " + child_n.attributes[j].nodeValue);
                        childObject.htmlElement[child_n.attributes[j].nodeName] =
                            child_n.attributes[j].nodeValue;
                    }
                    else
                    {
                        console.log("Key [" + child_n.attributes[j].nodeName + "] NOT FOUND");
                        //Injeta Name
                        childObject[child_n.attributes[j].nodeName] =
                            child_n.attributes[j].nodeValue;

                    }

                }
                //TODO inserir filhos da instancia ? tipo menu?
                //adiciona do DOM de objetos
                parent[childObject.Name] = childObject;

                /*
                Injeta uma propriedade Screen
                */
                childObject.Screen = screen;

                //adiciona do DOM da web
                parentElem.appendChild(childObject.htmlElement);
            }
            else
            {
                //nao eh custom
                var htmlElement = document.createElement(className);
                //vou setar os atruibutos deste element
                for (var j = 0; j < child_n.attributes.length; j++)
                {
                    htmlElement.setAttribute(child_n.attributes[j].name,
                        child_n.attributes[j].value);
                }
                Create(child_n, screen, parent, htmlElement);
                parentElem.appendChild(htmlElement);
            }
        }
        child_n = child_n.nextSibling;
    }
}

function ShowScreen(className)
{
    var parser0 = new DOMParser();
    var doc0 = parser0.parseFromString("<" + className + "></" + className + ">", "application/xml");
    var child_n = doc0.firstChild;

    if (Design[className])
    {
        var parser = new DOMParser();
        var doc = parser.parseFromString(Design[className], "application/xml");
        var nodeClass = doc.firstChild;
        //se nada for dito o controle eh sempre div

        var elemType = 'div';
        if (nodeClass.attributes["Class"])
        {
            //se tiver o atributo class eh tipo
            elemType = nodeClass.attributes["Class"].nodeValue;
        }
        var childObject = eval("new " + className + "();");
        
        //seta o elemento dom
        childObject.htmlElement = document.createElement(elemType);

        //Adiciona um link para editar esta pagina
        var editLink = document.createElement('a');
        editLink.target="_blank";
        editLink.href = "/edit/edit.html?name=" + className;
        editLink.innerText= "Edit";
        childObject.htmlElement.appendChild(editLink);
        ////

        var screen = childObject; //ele mesmo
        Create(nodeClass, screen, childObject, childObject.htmlElement);

        //seta os atributos da classe
        for (var j = 0; j < nodeClass.attributes.length; j++)
        {
            if (nodeClass.attributes[j].nodeName in childObject)
            {
                console.log("childObject[" + nodeClass.attributes[j].nodeName + "] = " + nodeClass.attributes[j].nodeValue);
                childObject[nodeClass.attributes[j].nodeName] =
                    nodeClass.attributes[j].nodeValue;
            }
            else if (nodeClass.attributes[j].nodeName in childObject.htmlElement)
            {
                console.log("childObject.htmlElement[" + nodeClass.attributes[j].nodeName + "] = " + nodeClass.attributes[j].nodeValue);
                childObject.htmlElement[nodeClass.attributes[j].nodeName] =
                    nodeClass.attributes[j].nodeValue;
            }
            else
            {
                console.log("Key [" + nodeClass.attributes[j].nodeName + "] NOT FOUND");
            }

        }
        //seta as propriedades da instancia
        for (var j = 0; j < child_n.attributes.length; j++)
        {
            if (child_n.attributes[j].nodeName in childObject)
            {
                console.log("childObject[" + child_n.attributes[j].nodeName + "] = " + child_n.attributes[j].nodeValue);
                childObject[child_n.attributes[j].nodeName] =
                    child_n.attributes[j].nodeValue;
            }
            else if (child_n.attributes[j].nodeName in childObject.htmlElement)
            {
                console.log("childObject.htmlElement[" + child_n.attributes[j].nodeName + "] = " + child_n.attributes[j].nodeValue);
                childObject.htmlElement[child_n.attributes[j].nodeName] =
                    child_n.attributes[j].nodeValue;
            }
            else
            {
                console.log("Key [" + child_n.attributes[j].nodeName + "] NOT FOUND");
                //Injeta Name
                childObject[child_n.attributes[j].nodeName] =
                    child_n.attributes[j].nodeValue;

            }
        }
    }

    document.body.innerText = "";
    document.body.appendChild(childObject.htmlElement);

    if ("OnShow" in screen)
    {
        childObject.OnShow();
    }

    return childObject;
}
////////

//Runtime
/**
 * xmlhttp.open("POST", "/json-handler");
   xmlhttp.setRequestHeader("Content-Type", "application/json;charset=UTF-8");
   xmlhttp.send(JSON.stringify({ "email": "hello@user.com", "response": { "name": "Tester" } }));
 */
 
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
                                if (xhr.responseText)
                                {
                                  responseJson = JSON.parse(xhr.responseText);
                                }
                                else
                                {
                                  responseJson = {};
                                }
                            }
                            catch (er) {
                                if (xhr_status == 200) {
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
