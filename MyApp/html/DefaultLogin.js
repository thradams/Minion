Design.DefaultLogin = `<DefaultLogin className="DefaultLogin">
    <h1>Login</h1>
    <MyButton onclick = "ShowScreen('Tela1')" Text="Voltar"></MyButton>

    <EditBox Name="UserName">Name</EditBox>
    <EditBox Name="Password">Name</EditBox>
     <MyButton onclick = "this.Screen.Login()" Text="Login"></MyButton>

</DefaultLogin>
`;

class DefaultLogin
{
  Login()
  {
      alert(this.UserName.htmlElement.value);
  }
}