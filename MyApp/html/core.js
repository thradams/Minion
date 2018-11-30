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

function ShowScreen(screenName)
{
  var parser = new DOMParser();
  var doc = parser.parseFromString("<" + screenName + "></" + screenName +
    ">", "application/xml");
  var nodeInstance = doc.firstChild;
  //A tela vai ser sempre uma div
  var htmlElement = document.createElement('div');
  var screen = eval("new " + screenName + "();");
  screen.htmlElement = htmlElement;

  var doc2 = parser.parseFromString(Design[screenName], "application/xml");
  var nodeClass = doc2.firstChild;
  Create(nodeClass, screen, screen, htmlElement);

  document.body.innerText = "";
  document.body.appendChild(screen.htmlElement);

  if ("OnShow" in screen)
  {
    screen.OnShow();
  }

  return screen;
}
