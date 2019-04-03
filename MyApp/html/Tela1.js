
class Tela1 {

    F() {
        //alert("teste");
        ShowScreen("Tela2");
    }

    OnShow() {
        //this.Button1.Text = "a";
    }
}

Tela1.Template=`<Tela1>
    <h1>Teste</h1>
    <MyButton Name="Button1" onclick = "this.Screen.F()"></MyButton>
    <MyButton  onclick = "ShowScreen('DefaultLogin')"></MyButton>
</Tela1>`;

