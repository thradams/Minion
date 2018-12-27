
class TopNavigationItem {

    set Text(s) {
        this.htmlElement.innerText = s;
    }

    set Active(b) {
        if (b) {
            this.htmlElement.className = "active";
        }
        else {
            this.htmlElement.className = "";
        }
    }
}
