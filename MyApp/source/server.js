
//proxy function
function func(s, callback) 
{ 
    HttpRequest("/edit/func/1", 'GET', function(json)
    {
      callback(null);
    },
    function (e){
      callback("error");
    });  
}
