

class DefaultLogin
{
  Login()
  {
      Call("LoginScript2", 
           this.UserName.htmlElement.value,
           this.Password.htmlElement.value,
          function (err, data)
          {
              if (err)
              {

              }
              else
              {
                  alert(data);
              }
            
          });          
  }
}
