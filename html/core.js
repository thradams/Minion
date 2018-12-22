var Design = {};


function CreateWebElement(node, screenObject, parentObject, parentHtmlElement)
{
  

  var htmlElement = document.createElement(node.nodeName);
  
  for (var j = 0; j < node.attributes.length; j++)
  {
    htmlElement.setAttribute(node.attributes[j].name, node.attributes[j].value);
  }

  if (node.attributes["Name"])
  {
    var childName = node.attributes["Name"].nodeValue;
    parentObject[childName] = htmlElement;
  }

  CreateCustom(node, screenObject, parentObject, htmlElement, false);

  parentHtmlElement.appendChild(htmlElement);
}


function CreateCustom(instanceNode, screenObject, thisObject, parentHtmlElement, setProperties)
{
  //Adiciona os filhos do objeto
  var child_n = instanceNode.firstChild;
  while (child_n)
  {
    if (child_n.nodeType === 3) /*#text*/
    {
      var txtNode = document.createTextNode(child_n.data);
      if (parentHtmlElement)
      {
        parentHtmlElement.appendChild(txtNode);
      }
    } else
    {
      var classNameChild = child_n.nodeName;
      if (Design[classNameChild])
      {
        var childObject = eval("new " + classNameChild + "();");

        var parser = new DOMParser();
        var doc = parser.parseFromString(Design[classNameChild], "application/xml");
        var templateNode = doc.firstChild;

        var elemType = 'div';
        if (templateNode.attributes["Class"])
        {
          elemType = templateNode.attributes["Class"].nodeValue;
        }
        
        if (child_n.attributes["Name"])
        {
          var childName = child_n.attributes["Name"].nodeValue;
          thisObject[childName] = childObject;
        }

        childObject.htmlElement = document.createElement(elemType);

        if (parentHtmlElement)
        {
          parentHtmlElement.appendChild(childObject.htmlElement);
        }

        if (screenObject && childObject !== screenObject)
        {
          childObject.Screen = screenObject;
        }
        
        CreateCustom(templateNode /*node*/,
          screenObject,
          childObject /*thisObject*/,
          childObject.htmlElement /*parentHtmlElement*/,
          true);

        CreateCustom(child_n /*node*/,
          screenObject,
          childObject /*thisObject*/,
          childObject.htmlElement /*parentHtmlElement*/,
          true);

          if ("OnCreated" in childObject)
          {
            childObject.OnCreated();
          }
      }
      else
      {
        CreateWebElement(child_n, screenObject, thisObject, parentHtmlElement);
      }
    }
    child_n = child_n.nextSibling;
  }

  if (setProperties)
  {
    for (var j = 0; j < instanceNode.attributes.length; j++)
    {
      if (instanceNode.attributes[j].nodeName in thisObject)
      {
        thisObject[instanceNode.attributes[j].nodeName] = instanceNode.attributes[j].nodeValue;
      }
      else if (instanceNode.attributes[j].nodeName in thisObject.htmlElement)
      {
        thisObject.htmlElement[instanceNode.attributes[j].nodeName] = instanceNode.attributes[j].nodeValue;
      }
      else
      {

      }
    }
  }
}

function InstanciateComponent(className)
{
  var childObject = eval("new " + className + "();");

  var parser = new DOMParser();
  var doc = parser.parseFromString(Design[className], "application/xml");
  var templateXML = doc.firstChild;

  var elemType = 'div';
  if (templateXML.attributes["Class"])
  {
    //se tiver o atributo class eh tipo
    elemType = node.attributes["Class"].nodeValue;
  }

  childObject.htmlElement = document.createElement(elemType);

  CreateCustom(templateXML,
    childObject /*screenObject*/,
    childObject,
    childObject.htmlElement);

  return childObject;
}

function ShowScreen(className)
{
  var childObject = InstanciateComponent(className);
  document.body.innerText = "";
  document.body.appendChild(childObject.htmlElement);

  console.log(JSON.stringify(childObject));
  if ("OnShow" in childObject)
  {    
    childObject.OnShow();
  }
}


