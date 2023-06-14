document.onreadystatechange = function () 
{
    if (document.readyState == "complete") 
    {  
        window.onscroll = function() {change_nav_color()};
        window.onresize = function() {change_nav_color()};
        var header = document.getElementById("nav");
        var sticky = header.offsetTop;

        function change_nav_color() 
        {
            if (document.documentElement.clientWidth >= "789")
            {
                if (window.pageYOffset > sticky) 
                {
                    document.getElementById("nav").style.backgroundColor = "rgba(0, 0, 0, 1)";
                } 
                else 
                { 
                    document.getElementById("nav").style.backgroundColor = "rgba(0, 0, 0, .5)";
                }
            }
            else
            {
                document.getElementById("nav").style.backgroundColor = "rgba(0, 0, 0, 1)";
            }
        }
    }
}