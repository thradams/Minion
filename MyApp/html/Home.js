class Home {

    OnNavigation(index) {
        this.MyText.innerText = "Menu #" + index + " clicked";
    }
}

function Main() {
    ShowScreen("Home");
}
Home.Template=`<Home>

  <TopNavigation>

    <TopNavigationItem Text="Home23" Active="true" />
    <TopNavigationItem Text="News" />
    <TopNavigationItem Text="Contact" />
  

  </TopNavigation>

  <div style="padding-left:16px">
    <h2>Top Navigation Example 3</h2>
    <p Name="MyText">Some content..</p>
   

  </div>



</Home>`;

