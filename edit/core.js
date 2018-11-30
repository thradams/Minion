var localxml = {};

function CreateHtmlElements(node, isSvgChild)
{
  var htmlElement = null;
  if (node.nodeName == "svg" || isSvgChild)
  {
    isSvgChild = true;
    htmlElement = document.createElementNS("http://www.w3.org/2000/svg", node.nodeName);
  }
  else
  {
    htmlElement = document.createElement(node.nodeName);
  }
  for (var j = 0; j < node.attributes.length; j++)
  {
    htmlElement.setAttribute(node.attributes[j].name, node.attributes[j].value);
  }
  var childCount = 0;
  var i = node.firstChild;
  while (i)
  {
    if (i.nodeType == 3)
    {
      var txtNode = document.createTextNode(i.data);
      htmlElement.appendChild(txtNode);
    }
    else
    {
      childCount++;
      var childHtmlElement = CreateHtmlElements(i, isSvgChild);
      htmlElement.appendChild(childHtmlElement);
    }
    i = i.nextSibling;
  }
  return htmlElement;
}
function CreateClass(className, isScreen, screen)
{
  var obj = null;
  var htmlElement = null;
  var xmlText = localxml[className];
  if (xmlText)
  {
    obj = eval("new " + className + "();");
    var parser = new DOMParser();
    var doc = parser.parseFromString(xmlText, "application/xml");
    if (isScreen)
    {
      htmlElement = document.createElement('div');

      if (screen == null)
      {
        screen = obj;
      }
    }
    else
    {
    }
    var i = doc.firstChild.firstChild;
    while (i)
    {
      if (i.nodeType != 3)
      {
        var childXmlText = localxml[i.nodeName];
        if (childXmlText)
        {
          var newChild = CreateClass(i.nodeName, false, screen);
          htmlElement.appendChild(newChild.domElem);
          for (var j = 0; j < i.attributes.length; j++)
          {
            if (i.attributes[j].name == 'Name')
            {
              obj[i.attributes[j].value] = newChild;
            }
            else
            {
              newChild[i.attributes[j].name] = i.attributes[j].value;
            }
          }
        }
        else
        {
          var childHtmlElement = CreateHtmlElements(i, false);
          if (htmlElement == null)
          {
            htmlElement = childHtmlElement;
          }
          else
          {
            htmlElement.appendChild(childHtmlElement);
          }
        }
      }
      i = i.nextSibling;
    }
  }
  else
  {
  }
  if (obj)
  {
    obj.domElem = htmlElement;
    obj.Screen = screen;
    if (obj.Initialized)
    {
      obj.Initialized();
    }


    if (isScreen)
    {
      for (var j = 0; j < doc.firstChild.attributes.length; j++)
      {
        console.log(doc.firstChild.attributes[j].NodeName);
        obj[doc.firstChild.attributes[j].nodeName] = doc.firstChild.attributes[j].nodeValue;
      }
    }
  }


  return obj;
}
function ShowScreen(name)
{
  if (!localxml[name])
  {
    var xhr = new XMLHttpRequest();
    xhr.open('GET', name + '.xml', true);
    xhr.onload = function ()
    {
      if (xhr.readyState === xhr.DONE)
      {
        if (xhr.status === 200)
        {
          localxml[name] = xhr.response;
          var e = CreateClass(name, true, null);
          document.body.innerText = "";
          document.body.appendChild(e.domElem);
        }
      }
    };
    xhr.send(null);
  }
  else
  {
    var e = CreateClass(name, true, null);
    document.body.innerText = "";
    document.body.appendChild(e.domElem);
  }
}
