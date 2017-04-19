$app_h_path=$ARGV[0];
$att_list_path=$ARGV[1];
$spp_uuid_list_path=$ARGV[2];
$hid_uuid_list_path=$ARGV[3];
$sched_path=$ARGV[4];
###############################
#YC_APP.h文件中有无定义APP_HID或者APP_SPP
$define_HID=0;
$define_SPP=0;
$define_GATT=0;
open(APP_H,$app_h_path)||die("Cannot open files\n");
@APP_H=<APP_H>;
$APP_H_length=@APP_H;
for($i;$i<$APP_H_length;$i++)
{
	@Temporary_variablesplit=split(/\/\//,@APP_H[$i]);
	$APP_H=@Temporary_variablesplit[0];
	#print $APP_H."\n";
	if($APP_H=~/#define /)
	{
		if($APP_H=~/APP_HID/)
		{
			#print "#define APP_HID";
			$define_HID=1;
		}
		if($APP_H=~/APP_SPP/)
		{
			#print "#define APP_SPP";
			$define_SPP=1;
		}
		if($APP_H=~/APP_LE_CLIENT/)
		{
			#print "#define APP_GATT";
			$define_GATT=1;
		}
	}
	
}
close(APP_H);
#############################
#都没有定义错误
if($define_HID==0 && $define_SPP==0 && $define_GATT==0)
{
	print("***********************************************\n");
	print("***********************************************\n");
	print("ERROR:not define APP_HID , APP_SPP and APP_GATT\n");
	print("***********************************************\n");
	print("***********************************************\n");
	die("");
}
#############################

#############################
#读取att_list.dat，有无内容判断
if($define_GATT==1)
{
	open(ATT_List,$att_list_path)||die("Cannot open files\n");
	@List=<ATT_List>;
	close(ATT_List);
}
else
{
	@List="";
	#print("***********************************************\n");
	#print("***********************************************\n");
	#print("WARNING:not define APP_GATT\n");
	#print("***********************************************\n");
	#print("***********************************************\n");
}
$ATTListEmpty=1;
foreach $List(@List)
{
	if($List =~ /[0-9a-fA-F]/)
	{
		$ATTListEmpty=0;
	}
}
if($ATTListEmpty==1 && $define_GATT==1)
{
	print ("att_list.dat file is empty\n");
}
#print "ATTListEmpty==".$ATTListEmpty;
####################################
if($define_HID==1)
{
	open(Uuid_List,$hid_uuid_list_path)||die("Cannot open files\n");
	@Uuid_List=<Uuid_List>;
	close(Uuid_List);
}
elsif($define_SPP==1)
{
	open(Uuid_List,$spp_uuid_list_path)||die("Cannot open files\n");
	@Uuid_List=<Uuid_List>;
	close(Uuid_List);
}
else
{
	@Uuid_List="";
	#print("***********************************************\n");
	#print("***********************************************\n");
	#print("WARNING:not define APP_SPP and APP_HID\n");
	#print("***********************************************\n");
	#print("***********************************************\n");
}
####################################
$UuidListEmpty=1;
foreach $Uuid_List(@Uuid_List)
{
	if($Uuid_List =~ /[0-9a-fA-F]/)
	{
		$UuidListEmpty=0;
	}
}
if($UuidListEmpty==1 && $define_SPP==1 && $define_HID==1)
{
	print ("uuid_list.dat file is empty\n");
}
$fileExist = -e $sched_path;
if ($fileExist)
{ 
	open(Sched,$sched_path)||die("Cannot open files\n");
	@Sched = <Sched>;
	close(Sched);
	open(Sched,">$sched_path")||die("Cannot open files\n");
	@OriginalRecord[0]="";
	$Data_num=0;
	foreach $sched (@Sched)
	{
		next if($sched=~//);
		if($sched=~/[0-9a-fA-F][0-9a-fA-F][0-9a-fA-F][0-9a-fA-F]:/)
		{
			$Data_num++;
			@OriginalRecord[$Data_num]=$sched;
		}
		else
		{
			@OriginalRecord[$Data_num].=$sched;
		}
	}
	#print Sched @OriginalRecord;
	foreach $Sched(@OriginalRecord)
	{
		if($Sched=~/4504:/)
		{
			if($ATTListEmpty==0)
			{
				#print Sched ("\n");
			} 
			else
			{
				#print Sched $Sched;
			}
		}
		elsif($Sched=~/4814:/)
		{
			if($UuidListEmpty==0)
			{
				#print Sched ("\n");
			} 
			else
			{
				#print Sched $Sched;
			}
		}
		else
		{
			print Sched $Sched;
		}
	}
	close(Sched);
	open(Sched,">>$sched_path")||die("Cannot open files\n");
	if($ATTListEmpty==0)
	{
		$List="4504:\n";
		foreach $text(@List)
		{
			$List.=$text;
		}
		$List.="\n";
		print Sched $List;
	}
	if($UuidListEmpty==0)
	{
		$List="4814:\n";
		foreach $text(@Uuid_List)
		{
			$List.=$text;
		}
		$List.="\n";
		print Sched $List;
	}
	close(Sched);
}
else 
{    
	print "canot find sched.rom file\n";
}

#######################################
#结果显示
print("***********************************************\n");
print("***********************************************\n");
print "Current application is ";
if($define_GATT==1)
{
	print "LE_CLIENT ";
	if($define_HID==1)
	{
		print "and HID";
	}
	elsif($define_SPP==1)
	{
		print "and SPP";
	}
}
else
{
	if($define_HID==1)
	{
		print "HID";
	}
	elsif($define_SPP==1)
	{
		print "SPP";
	}
}
print "\n";
print("***********************************************\n");
print("***********************************************\n");