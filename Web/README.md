Video call is an open source sample project provided by BytePlus RTC. This topic describes how to run this sample project to try the RTC video call.

## **Note**

After using the project file to build an application, you can use the built application for video calling.
You and your colleagues must join the same room to have a video calling.

## **Prerequisites**

- [BytePlus developer account](https://console.byteplus.com/auth/login/)

- yarn

## **Project setup**

### **Step 1: Contact BytePlus Technical Support to create an account in the** **RTC** **console**

### **Step 2: Obtain AppID and AppKey**

Obtain the AppID and AppKey by creating an application on the [App Management](https://console.byteplus.com/rtc/listRTC) page or using the created application in the BytePlus RTC console.

### **Step 3: Obtain AccessKey** **ID** **and SecretAccessKey**

Obtain the **AccessKeyID and SecretAccessKey** on the [Key Management](https://console.byteplus.com/iam/keymanage) page in the BytePlus RTC console.

### **Step 4: Build project**

1.  Go to the directory `BytePlus_VideoCallDemo/src/appConfig.ts`

2.  Enter **HOST**.<br>
You can use **https://demo.byteplus.com/rtc/demo/rtc_demo_special** as the test domain name of the server. However, this domain name only provides testing running and cannot be used for official operations.

   <img src="https://portal.volccdn.com/obj/volcfe/cloud-universal-doc/upload_b5ff5917f41d5867efbc12bcecc5411b.jpg" width="500px" >

3.  **Enter AppID, AppKey, AccessKeyID, and SecretAccessKey.** <br>
    Enter the **AppID, AppKey, AccessKeyID**, and **SecretAccessKey** obtained from the BytePlus RTC console in the corresponding fields.<br>

        <img src="https://portal.volccdn.com/obj/volcfe/cloud-universal-doc/upload_05ae5a5104a77e18d39246a9b939430f.png" width="500px" >

4.  Open a CLI on a terminal and go to the directory `BytePlus_VideoCallDemo`.

<img src="https://portal.volccdn.com/obj/volcfe/cloud-universal-doc/upload_b901b6a1302d6b1bc9a16292482cc145.jpg" width="500px" >

5.  Run the `yarn` command to build the project.<br>
    <img src="https://portal.volccdn.com/obj/volcfe/cloud-universal-doc/upload_52fda9f97a7489a39cd6967fed3b7fdc.jpg" width="500px" >

### **Step 5: Run**

Run the `yarn dev` command to run the project.<br>
<img src="https://portal.volccdn.com/obj/volcfe/cloud-universal-doc/upload_d76b74d9eaa90028dcbee241920b8c31.jpg" width="500px" >

The following figure shows the screen when the app starts running.

<img src="https://portal.volccdn.com/obj/volcfe/cloud-universal-doc/upload_0139e8ddd9c21ff6705a7c073f7d9ad6.jpg" width="500px" >
