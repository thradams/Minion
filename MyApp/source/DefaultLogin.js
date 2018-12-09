

class DefaultLogin
{
  Login()
  {
      Call("func", this.UserName.htmlElement.value,
          this.Password.htmlElement.value,
          function (e)
      {
          alert(e);           
      });          
  }
}
