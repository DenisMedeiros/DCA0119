<?php 

// Open database.

$path = (dirname(__FILE__).'/database.sqlite');
$dir = 'sqlite:'.$path;
$dbh  = new PDO($dir) or die("cannot open the database");


// Create table if it does not exist.
$query =  "CREATE TABLE IF NOT EXISTS Entry (
id INTEGER NOT NULL PRIMARY KEY,
current_time DATETIME NOT NULL,
last_avg_temp FLOAT NOT NULL
);";

$dbh->query($query);


if ($_SERVER['REQUEST_METHOD'] === 'POST') 
{
    if(isset($_POST['current_time']) && isset($_POST['last_avg_temp'])) 
    {
        $current_time = $_POST["current_time"];
        $last_avg_temp = (float) $_POST["last_avg_temp"];
        
        // Save in DB the received data.
        $query = "INSERT INTO Entry(current_time, last_avg_temp) VALUES ('$current_time', $last_avg_temp)";
        $result = $dbh->query($query);
    } 
    else
    {
        echo "Missing fields";
    }
    
    exit();
}
else if ($_SERVER['REQUEST_METHOD'] === 'GET')
{
    // Get the last 10 entries.
    $query = "SELECT * FROM Entry ORDER BY id DESC LIMIT 10;";
    
    $sth = $dbh->prepare($query);
    $sth->execute();
    $rows = $sth->fetchAll();
}
?>



<!DOCTYPE html>
<html lang="en">

<head>
    
    <meta http-equiv="refresh" content="30" />

    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <meta name="description" content="">
    <meta name="author" content="">

    <title> DataCenterMonitor</title>

    <!-- Bootstrap Core CSS -->
    <link href="./vendor/bootstrap/css/bootstrap.min.css" rel="stylesheet">

    <!-- MetisMenu CSS -->
    <link href="./vendor/metisMenu/metisMenu.min.css" rel="stylesheet">

    <!-- Custom CSS -->
    <link href="./dist/css/sb-admin-2.css" rel="stylesheet">

    <!-- Morris Charts CSS -->
    <link href="./vendor/morrisjs/morris.css" rel="stylesheet">

    <!-- Custom Fonts -->
    <link href="./vendor/font-awesome/css/font-awesome.min.css" rel="stylesheet" type="text/css">

    <!-- HTML5 Shim and Respond.js IE8 support of HTML5 elements and media queries -->
    <!-- WARNING: Respond.js doesn't work if you view the page via file:// -->
    <!--[if lt IE 9]>
        <script src="https://oss.maxcdn.com/libs/html5shiv/3.7.0/html5shiv.js"></script>
        <script src="https://oss.maxcdn.com/libs/respond.js/1.4.2/respond.min.js"></script>
    <![endif]-->

</head>

<body>

    <div id="wrapper">

        <!-- Navigation -->
        <nav class="navbar navbar-default navbar-static-top" role="navigation" style="margin-bottom: 0">
            <div class="navbar-header">
                <button type="button" class="navbar-toggle" data-toggle="collapse" data-target=".navbar-collapse">
                    <span class="sr-only">Toggle navigation</span>
                    <span class="icon-bar"></span>
                    <span class="icon-bar"></span>
                    <span class="icon-bar"></span>
                </button>
                <a class="navbar-brand" href="index.html">DataCenterMonitor</a>
            </div>
            <!-- /.navbar-header -->

            <ul class="nav navbar-top-links navbar-right">
                <li class="dropdown">
                    <a class="dropdown-toggle" data-toggle="dropdown" href="#">
                        <i class="fa fa-envelope fa-fw"></i> <i class="fa fa-caret-down"></i>
                    </a>
                    <ul class="dropdown-menu dropdown-messages">
                    </ul>
                    <!-- /.dropdown-messages -->
                </li>
                <!-- /.dropdown -->
                <li class="dropdown">
                    <a class="dropdown-toggle" data-toggle="dropdown" href="#">
                        <i class="fa fa-tasks fa-fw"></i> <i class="fa fa-caret-down"></i>
                    </a>
                    <ul class="dropdown-menu dropdown-tasks">
                    </ul>
                    <!-- /.dropdown-tasks -->
                </li>
                <!-- /.dropdown -->
                <li class="dropdown">
                    <a class="dropdown-toggle" data-toggle="dropdown" href="#">
                        <i class="fa fa-bell fa-fw"></i> <i class="fa fa-caret-down"></i>
                    </a>
                    <ul class="dropdown-menu dropdown-alerts">
                    </ul>
                    <!-- /.dropdown-alerts -->
                </li>
                <!-- /.dropdown -->
                <li class="dropdown">
                    <a class="dropdown-toggle" data-toggle="dropdown" href="#">
                        <i class="fa fa-user fa-fw"></i> <i class="fa fa-caret-down"></i>
                    </a>
                    <ul class="dropdown-menu dropdown-user">
                        <li><a href="#"><i class="fa fa-user fa-fw"></i> User Profile</a>
                        </li>
                        <li><a href="#"><i class="fa fa-gear fa-fw"></i> Settings</a>
                        </li>
                        <li class="divider"></li>
                        <li><a href="login.html"><i class="fa fa-sign-out fa-fw"></i> Logout</a>
                        </li>
                    </ul>
                    <!-- /.dropdown-user -->
                </li>
                <!-- /.dropdown -->
            </ul>
            <!-- /.navbar-top-links -->

            <div class="navbar-default sidebar" role="navigation">
                <div class="sidebar-nav navbar-collapse">
                    <ul class="nav" id="side-menu">
                        <li class="sidebar-search">
                            <div class="input-group custom-search-form">
                                <input type="text" class="form-control" placeholder="Search...">
                                <span class="input-group-btn">
                                <button class="btn btn-default" type="button">
                                    <i class="fa fa-search"></i>
                                </button>
                            </span>
                            </div>
                            <!-- /input-group -->
                        </li>
                        <li>
                            <a href="#table"><i class="fa fa-table fa-fw"></i> Table </a>
                        </li>
                        <li>
                            <a href="#chart"><i class="fa fa-bar-chart-o fa-fw"></i> Chart </a>
                        </li>
                    </ul>
                </div>
                <!-- /.sidebar-collapse -->
            </div>
            <!-- /.navbar-static-side -->
        </nav>

        <div id="page-wrapper">
            <div class="row">
                <div class="col-lg-12">
                    <h1 class="page-header">Last Temperatures </h1>
                </div>
                <!-- /.col-lg-12 -->
            </div>
 
            <div class="panel panel-default">
                <div class="panel-heading">
                    <a name="table"> Table </a>
                </div>
                <!-- /.panel-heading -->
                <div class="panel-body">
                    <table width="100%" class="table table-striped table-bordered table-hover" id="dataTables-example">
                        <thead>   
                            <tr>
                                <th>Entry date/time</th>
                                <th>Temperature</th>
                            </tr>
                        </thead>
                        <tbody>
                            <?php foreach ($rows as $row) { 
                                $datetime = new DateTime($row['current_time']);
                                $dateStr = $datetime->format('d/m/y H:i:s');
                                $temperature = (float) $row['last_avg_temp'];
                            ?>
                                <tr class="odd" <?php if ($temperature > 27) { echo "style=\"color: red\""; } ?> >
                                    <td> <?php echo $dateStr; ?> </td>
                                    <td> <?php echo $temperature; ?> °C </td>
                                </tr>
                            <?php } ?>
                        </tbody>
                    </table>
                    <!-- /.table-responsive -->
                </div>
            </div>

            <!-- /.row -->
            <div class="row">
                <div class="col-lg-12">
                    <div class="panel panel-default">
                        <div class="panel-heading">
                            <a name="chart"> Line Chart </a>
                        </div>
                        <!-- /.panel-heading -->
                        <div class="panel-body">
                            <div class="flot-chart">
                                <div class="flot-chart-content" id="flot-line-chart"></div>
                            </div>
                        </div>
                        <!-- /.panel-body -->
                    </div>
                    <!-- /.panel -->
                </div>
                <!-- /.col-lg-6 -->
            </div>
            <!-- /.row -->
    
        </div>
        <!-- /#page-wrapper -->
    </div>
    <!-- /#wrapper -->

    <!-- jQuery -->
    <script src="./vendor/jquery/jquery.min.js"></script>

    <!-- Bootstrap Core JavaScript -->
    <script src="./vendor/bootstrap/js/bootstrap.min.js"></script>

    <!-- Metis Menu Plugin JavaScript -->
    <script src="./vendor/metisMenu/metisMenu.min.js"></script>

    <!-- Flot Charts JavaScript -->
    <script src="./vendor/flot/excanvas.min.js"></script>
    <script src="./vendor/flot/jquery.flot.js"></script>
    <script src="./vendor/flot/jquery.flot.pie.js"></script>
    <script src="./vendor/flot/jquery.flot.resize.js"></script>
    <script src="./vendor/flot/jquery.flot.time.js"></script>
    <script src="./vendor/flot-tooltip/jquery.flot.tooltip.min.js"></script>

    <!-- Custom Theme JavaScript -->
    <script src="./dist/js/sb-admin-2.js"></script>
    
    <script>
        $(document).ready(function() {
            
            var dateTimes = [];
            
            <?php foreach ($rows as $row) { 
                $datetime = new DateTime($row['current_time']);
                $timestamp =  1000 * $datetime->getTimestamp();
                $temperature = (float) $row['last_avg_temp'];
            ?>
                dateTimes.push([<?php echo $timestamp; ?>, <?php echo $temperature; ?>]);
            <?php } ?>

            var offset = 0;
            plot();

            function plot() {
                var sin = [];
                
                var options = {
                    series: {
                        lines: {
                            show: true
                        },
                        points: {
                            show: true
                        }
                    },
                    grid: {
                        hoverable: true //IMPORTANT! this is needed for tooltip to work
                    },
                    xaxis: {
                        mode: "time",
                        timeformat: "%d/%m/%y %H:%M:%S",
                        timezone: "browser"
                    },
                    yaxis: {
                        min: 0, max: 40
                    },
                    tooltip: true,
                    tooltipOpts: {
                        content: "'%s' at %x.1 is %y.1 °C",

                    }
                };

                var plotObj = $.plot($("#flot-line-chart"), [
                    {
                        data: dateTimes,
                        label: "temp"
                    },
                ], options);
            }
        });
        
    </script>

</body>

</html>
