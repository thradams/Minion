class MyButton {

    set Text(s) {
        this.htmlElement.innerText = s;
    }

    get Text() {
        return this.htmlElement.innerText;
    }

    set onclick(txt) {

        var _this = this;
        this.f = new Function(txt);
        this.htmlElement.onclick = function () {
            /*isso faz com que o this seja o MyButton e nao o htmlelement*/
            _this.f();
        };
    }
}

MyButton.Template=`<MyButton Class = "button" className="MyButton" Text="Button"></MyButton>`;

