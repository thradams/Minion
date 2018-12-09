

function LoginScript2(user, password, callback) 
{
    LoginNative(user, password, callback);    
}

function LoginScript(user, password, callback) 
{
    print("LoginScript(" + user + "," + password + ")");

    //retorna uma string, poderi ser um object
    callback("", "\"OK2\"");
}


function func(s, s2, callback) 
{ 
  print("I am at func, arg1 = " + s + s2);

  //retorna uma string, poderi ser um object
  callback("", "\"OK2\""); 
}
