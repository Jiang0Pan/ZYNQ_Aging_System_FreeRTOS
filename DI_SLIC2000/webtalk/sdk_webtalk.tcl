<<<<<<< .mine
webtalk_init -webtalk_dir E:\\system_arm\\Software\\DI_PROJECT\\CODE\\DI_v1.5\\webtalk
||||||| .r15
webtalk_init -webtalk_dir E:\\Benchmaking_DI\\code\\webtalk
=======
webtalk_init -webtalk_dir E:\\system_arm\\Software\\DI_PROJECT\\CODE\\DI_v1.2\\webtalk
>>>>>>> .r19
webtalk_register_client -client project
<<<<<<< .mine
webtalk_add_data -client project -key date_generated -value "星期二 三月 9 19:33:42 2021" -context "software_version_and_target_device"
||||||| .r15
webtalk_add_data -client project -key date_generated -value "星期一 三月 8 10:57:25 2021" -context "software_version_and_target_device"
=======
webtalk_add_data -client project -key date_generated -value "星期日 二月 7 18:53:36 2021" -context "software_version_and_target_device"
>>>>>>> .r19
webtalk_add_data -client project -key product_version -value "SDK v2015.4" -context "software_version_and_target_device"
webtalk_add_data -client project -key build_version -value "2015.4" -context "software_version_and_target_device"
webtalk_add_data -client project -key os_platform -value "amd64" -context "software_version_and_target_device"
webtalk_add_data -client project -key registration_id -value "" -context "software_version_and_target_device"
webtalk_add_data -client project -key tool_flow -value "SDK" -context "software_version_and_target_device"
webtalk_add_data -client project -key beta -value "false" -context "software_version_and_target_device"
webtalk_add_data -client project -key route_design -value "NA" -context "software_version_and_target_device"
webtalk_add_data -client project -key target_family -value "NA" -context "software_version_and_target_device"
webtalk_add_data -client project -key target_device -value "NA" -context "software_version_and_target_device"
webtalk_add_data -client project -key target_package -value "NA" -context "software_version_and_target_device"
webtalk_add_data -client project -key target_speed -value "NA" -context "software_version_and_target_device"
<<<<<<< .mine
webtalk_add_data -client project -key random_id -value "3dimi97ug1rgth2536ic7rc5rn" -context "software_version_and_target_device"
webtalk_add_data -client project -key project_id -value "2015.4_46" -context "software_version_and_target_device"
webtalk_add_data -client project -key project_iteration -value "46" -context "software_version_and_target_device"
||||||| .r15
webtalk_add_data -client project -key random_id -value "2va5o0pvgpds2heik5iol9cjpg" -context "software_version_and_target_device"
webtalk_add_data -client project -key project_id -value "2015.4_2" -context "software_version_and_target_device"
webtalk_add_data -client project -key project_iteration -value "2" -context "software_version_and_target_device"
=======
webtalk_add_data -client project -key random_id -value "86pnibfh4qo5ancuc2925go9e8" -context "software_version_and_target_device"
webtalk_add_data -client project -key project_id -value "2015.4_44" -context "software_version_and_target_device"
webtalk_add_data -client project -key project_iteration -value "44" -context "software_version_and_target_device"
>>>>>>> .r19
webtalk_add_data -client project -key os_name -value "" -context "user_environment"
webtalk_add_data -client project -key os_release -value "" -context "user_environment"
webtalk_add_data -client project -key cpu_name -value "" -context "user_environment"
webtalk_add_data -client project -key cpu_speed -value "" -context "user_environment"
webtalk_add_data -client project -key total_processors -value "" -context "user_environment"
webtalk_add_data -client project -key system_ram -value "" -context "user_environment"
webtalk_register_client -client sdk
<<<<<<< .mine
webtalk_add_data -client sdk -key uid -value "1615274308434" -context "sdk\\\\bsp/1615274308434"
webtalk_add_data -client sdk -key hwid -value "1615272814144" -context "sdk\\\\bsp/1615274308434"
webtalk_add_data -client sdk -key os -value "standalone" -context "sdk\\\\bsp/1615274308434"
webtalk_add_data -client sdk -key apptemplate -value "hello_world" -context "sdk\\\\bsp/1615274308434"
webtalk_add_data -client sdk -key uid -value "1615274311470" -context "sdk\\\\application/1615274311470"
webtalk_add_data -client sdk -key hwid -value "1615272814144" -context "sdk\\\\application/1615274311470"
webtalk_add_data -client sdk -key bspid -value "1615274308434" -context "sdk\\\\application/1615274311470"
webtalk_add_data -client sdk -key newbsp -value "true" -context "sdk\\\\application/1615274311470"
webtalk_add_data -client sdk -key os -value "standalone" -context "sdk\\\\application/1615274311470"
webtalk_add_data -client sdk -key apptemplate -value "hello_world" -context "sdk\\\\application/1615274311470"
webtalk_add_data -client sdk -key uid -value "1615274364176" -context "sdk\\\\bsp/1615274364176"
webtalk_add_data -client sdk -key hwid -value "1615272814144" -context "sdk\\\\bsp/1615274364176"
webtalk_add_data -client sdk -key os -value "standalone" -context "sdk\\\\bsp/1615274364176"
webtalk_add_data -client sdk -key apptemplate -value "zynq_fsbl" -context "sdk\\\\bsp/1615274364176"
webtalk_add_data -client sdk -key uid -value "1615274371477" -context "sdk\\\\application/1615274371477"
webtalk_add_data -client sdk -key hwid -value "1615272814144" -context "sdk\\\\application/1615274371477"
webtalk_add_data -client sdk -key bspid -value "1615274364176" -context "sdk\\\\application/1615274371477"
webtalk_add_data -client sdk -key newbsp -value "true" -context "sdk\\\\application/1615274371477"
webtalk_add_data -client sdk -key os -value "standalone" -context "sdk\\\\application/1615274371477"
webtalk_add_data -client sdk -key apptemplate -value "zynq_fsbl" -context "sdk\\\\application/1615274371477"
webtalk_transmit -clientid 3753719381 -regid "" -xml E:\\system_arm\\Software\\DI_PROJECT\\CODE\\DI_v1.5\\webtalk\\usage_statistics_ext_sdk.xml -html E:\\system_arm\\Software\\DI_PROJECT\\CODE\\DI_v1.5\\webtalk\\usage_statistics_ext_sdk.html -wdm E:\\system_arm\\Software\\DI_PROJECT\\CODE\\DI_v1.5\\webtalk\\sdk_webtalk.wdm -intro "<H3>SDK Usage Report</H3><BR>"
||||||| .r15
webtalk_add_data -client sdk -key uid -value "1615108198341" -context "sdk\\\\bsp/1615108198341"
webtalk_add_data -client sdk -key hwid -value "1615106496883" -context "sdk\\\\bsp/1615108198341"
webtalk_add_data -client sdk -key os -value "standalone" -context "sdk\\\\bsp/1615108198341"
webtalk_add_data -client sdk -key apptemplate -value "zynq_fsbl" -context "sdk\\\\bsp/1615108198341"
webtalk_add_data -client sdk -key uid -value "1615108210819" -context "sdk\\\\application/1615108210819"
webtalk_add_data -client sdk -key hwid -value "1615106496883" -context "sdk\\\\application/1615108210819"
webtalk_add_data -client sdk -key bspid -value "1615108198341" -context "sdk\\\\application/1615108210819"
webtalk_add_data -client sdk -key newbsp -value "true" -context "sdk\\\\application/1615108210819"
webtalk_add_data -client sdk -key os -value "standalone" -context "sdk\\\\application/1615108210819"
webtalk_add_data -client sdk -key apptemplate -value "zynq_fsbl" -context "sdk\\\\application/1615108210819"
webtalk_transmit -clientid 3676109432 -regid "" -xml E:\\Benchmaking_DI\\code\\webtalk\\usage_statistics_ext_sdk.xml -html E:\\Benchmaking_DI\\code\\webtalk\\usage_statistics_ext_sdk.html -wdm E:\\Benchmaking_DI\\code\\webtalk\\sdk_webtalk.wdm -intro "<H3>SDK Usage Report</H3><BR>"
=======
webtalk_add_data -client sdk -key uid -value "1612690825902" -context "sdk\\\\bsp/1612690825902"
webtalk_add_data -client sdk -key hwid -value "1611553663393" -context "sdk\\\\bsp/1612690825902"
webtalk_add_data -client sdk -key os -value "standalone" -context "sdk\\\\bsp/1612690825902"
webtalk_add_data -client sdk -key apptemplate -value "zynq_fsbl" -context "sdk\\\\bsp/1612690825902"
webtalk_add_data -client sdk -key uid -value "1612690834780" -context "sdk\\\\application/1612690834780"
webtalk_add_data -client sdk -key hwid -value "1611553663393" -context "sdk\\\\application/1612690834780"
webtalk_add_data -client sdk -key bspid -value "1612690825902" -context "sdk\\\\application/1612690834780"
webtalk_add_data -client sdk -key newbsp -value "true" -context "sdk\\\\application/1612690834780"
webtalk_add_data -client sdk -key os -value "standalone" -context "sdk\\\\application/1612690834780"
webtalk_add_data -client sdk -key apptemplate -value "zynq_fsbl" -context "sdk\\\\application/1612690834780"
webtalk_transmit -clientid 825258990 -regid "" -xml E:\\system_arm\\Software\\DI_PROJECT\\CODE\\DI_v1.2\\webtalk\\usage_statistics_ext_sdk.xml -html E:\\system_arm\\Software\\DI_PROJECT\\CODE\\DI_v1.2\\webtalk\\usage_statistics_ext_sdk.html -wdm E:\\system_arm\\Software\\DI_PROJECT\\CODE\\DI_v1.2\\webtalk\\sdk_webtalk.wdm -intro "<H3>SDK Usage Report</H3><BR>"
>>>>>>> .r19
webtalk_terminate
