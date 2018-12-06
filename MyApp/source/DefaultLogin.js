class DefaultLogin
{
  Login()
  {
    func(this.UserName.htmlElement.value, 
    function(er){
      if (!er){ alert(  "Ok"); }
    });    
  }
}
