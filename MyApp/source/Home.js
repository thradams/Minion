class Home {

    OnNavigation(index) {
        this.MyText.innerText = "Menu #" + index + " clicked";
    }
}

function Main() {
    ShowScreen("Home");
}