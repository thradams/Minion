Design.DefaultLogin = `<DefaultLogin className="DefaultLogin">
    <h1>Login</h1>
    <MyButton onclick = "ShowScreen('Tela1')" Text="Voltar"></MyButton>

  
  <div>
  <p>User</p>
  <EditBox Name="UserName">Name</EditBox> <br/>
  </div>
  
  <div>
    <p>Password</p>
    <EditBox Name="Password">Name</EditBox> <br/>
  </div>
    <MyButton onclick = "this.Screen.Login()" Text="Login"></MyButton>
  
  
</DefaultLogin>
`;

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
