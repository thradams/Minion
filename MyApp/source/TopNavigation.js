
class TopNavigation {

    constructor() {
        /** @type {HTMLDivElement} */
        this.htmlElement = null;
    }

    OnCreated() {
        var _this = this;
        this.htmlElement.onclick = function (ev) {
            var index = -1;
            for (var i = 0; i < _this.htmlElement.childElementCount; i++) {
                if (ev.target === _this.htmlElement.children[i]) {
                    _this.htmlElement.children[i].className = "active";
                    index = i;
                }
                else {
                    _this.htmlElement.children[i].className = "";
                }
            }
            ev.cancelBubble = true;
            if ("OnNavigation" in _this.Screen) {
                _this.Screen.OnNavigation(index);
            }
        }
    }
}
