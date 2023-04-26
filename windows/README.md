Video call is an open source sample project provided by BytePlus RTC. This topic describes how to run this sample project to try video calling.

## **Note**

After using the project file to build an application, you can use the built application for Video call.
You and your colleagues must join the same room to have a video calling.
This open-source project doesn't support Effects-related function. Please download [BytePlus RTC APP](https://docs.byteplus.com/byteplus-rtc/docs/75707#download-solution-demo) to experience it.

## Prerequisites

- Windows 7+, recommended Windows 10 system

- Visual Studio 2017+

- Qt 5.9+

- Qt runtime environment corresponding to the version of Visual Studio (such as Visual Studio 2017 corresponds to msvc 2017)

- cmake 3.14+

- C++ as the development language, supporting version C++ 11

- [BytePlus developer account](https://console.byteplus.com/auth/login/)

## **Project setup**

### **Step 1: Obtain AppID and AppKey**

Obtain the AppID and AppKey by creating an application on the [App Management](https://console.byteplus.com/rtc/listRTC) page or using the created application in the BytePlus RTC console.

### Step 2: Obtain AccessKey ID and SecretAccessKey

Obtain the **AccessKeyID and SecretAccessKey** on the [Key Management](https://console.byteplus.com/iam/keymanage) page in the BytePlus RTC console.


### **Step 3: Environment variables configuration**

Configure the QT environment variables `Qt32Path`, `Qt64Path`.

> The values of `Qt32Path`, `Qt64Path` are the corresponding cmake folder path under your QT installation path.

<img src="https://lf3-static.bytednsdoc.com/obj/eden-cn/pkupenuhr/variable.jpg" width="500px">

### **Step 4: Configure Demo project file**

Open cmd, enter the directory where the installation package code is downloaded, and execute `cmake -G "Visual Studio 16" -Bbuild_win -A "Win32"` (32-bit) or `cmake -G "Visual Studio 16" -Bbuild_win -A "x64" `(64-bit) command to generate a project in the `build_win` directory.

> The `16` in the command `cmake -G "Visual Studio 16" -Bbuild_win` is the version number corresponding to the version of Visual Studio you are using. For example, when using Visual Studio 2019, the value is `16`, and when using Visual Studio 2017, the value is `15`. For the corresponding version numbers of each version of Visual Studio, please refer to: [Visual Studio build numbers and release dates](https://docs.microsoft.com/zh-cn/visualstudio/install/visual-studio-build-numbers-and-release-dates?view=vs-2019).

<img src="https://lf3-static.bytednsdoc.com/obj/eden-cn/pkupenuhr/img_v2_7f60d105-5e4f-4567-a62d-aa2734f599eg.jpg" width="500px">


### Step 5: Fill in **URL**

 You can use **https://demo.byteplus.com/rtc/demo/rtc_demo_special/login** as the test domain name of the server. However, this domain name only provides testing running and cannot be used for official operations.<br>

<img src="https://lf3-static.bytednsdoc.com/obj/eden-cn/pkupenuhr/urlinput.jpg" width="800px">


### Step 6: Enter AppID, AppKey, AccessKeyID, and SecretAccessKey.

Enter the **AppID, AppKey, AccessKeyID**, and **SecretAccessKey** obtained from the BytePlus RTC console in the corresponding fields of the file `BuildConfig.h`.<br>
    <img src="https://lf3-static.bytednsdoc.com/obj/eden-cn/pkupenuhr/windows.png" width="800px">

### **Step 7: Compile and run**

1. Enter the `RTC_VideoCall_Demo/windows/RTCSolution/build_win` directory, and use Visual Studio to open the project file `BytePlus.sln`.

<img src="https://lf3-static.bytednsdoc.com/obj/eden-cn/pkupenuhr/4.jpg" width="500px">

2. Set BytePlusRTC as the startup project.

<img src="https://lf3-static.bytednsdoc.com/obj/eden-cn/pkupenuhr/2.jpg" width="500px">

3. Click [Debug] -> [Start Debugging] on the upper menu bar of Visual Studio to start compiling and running the project.

<img src="https://lf3-static.bytednsdoc.com/obj/eden-cn/pkupenuhr/3.jpg" width="800px">


The running interface is as follows:

<img src="https://lf3-static.bytednsdoc.com/obj/eden-cn/pkupenuhr/welcome.jpg" width="600px"></div>