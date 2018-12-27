Design.Home = `<Home>

  <TopNavigation>

    <TopNavigationItem Text="Home" Active="true" />
    <TopNavigationItem Text="News" />
    <TopNavigationItem Text="Contact" />
    <TopNavigationItem Text="About" />



  </TopNavigation>

  <div style="padding-left:16px">
    <h2>Top Navigation Example</h2>
    <p Name="MyText">Some content..</p>
   

  </div>



</Home>`;

class Home {

    OnNavigation(index) {
        this.MyText.innerText = "Menu #" + index + " clicked";
    }
}

function Main() {
    ShowScreen("Home");
}