# 常见问题

* Could Not find Vulkan (missing: VULKAN_LIBRARY VULKAN_INCLUDE_DIR)
  * **`原因`**：可能刚安装`Vulkan SDK`，此时`CMake`没有更新相关环境
  * **`解决方法`**：尝试重启电脑 或者 配置`CMake`中如图属性：
    ![dsa](./images/FQA_CMakeCannotFindVulkan.png)

* Could Not find BASH (missing: BASH_EXECUTABLE)
  * **`原因`**：与`Could Not find Vulkan (missing: VULKAN_LIBRARY VULKAN_INCLUDE_DIR)`问题类似，`CMake`中设置`BASH_EXECUTABLE`参数
  * **`解决方法`**：与`Could Not find Vulkan (missing: VULKAN_LIBRARY VULKAN_INCLUDE_DIR)`问题类似，`CMake`中设置`BASH_EXECUTABLE`参数

  *注：`BASH_EXECUTABLE`指的是`git`目录下的`bash.exe`，一般在`{git安装目录}/bin/bash.exe`目录下*

* CMake Error at thirdparty/CMakeList.txt:.. (add_subdirectory)
  * **`原因`**：从`github`上`clone`或是下载的压缩文件有时并不会将第三方库一起下载下来，这会导致`./thirdparty`目录下的各个第三方库为空文件夹。还有可能是我`Turbo`的`git`配置有问题
  * **`解决方法`**：两种方法，随便使用其一即可，推荐方法`2`
    1. 根据`./README.md`中的`Build`章节中的第三方库的链接，将第三方库都下载下来，之后直接替换`./thirdparty`下的文件夹
    2. 在`clone`下来的`Turbo`目录下执行如下指令，将会自动下载对应第三方库
        ```Cmd
        git submodule init
        git submodule update
        ```
* Expression: vector subscript out of range  

  ![dsa](./images/FQA_VectorSubscriptOutOfRange.png)

  * **`原因`**：运行的程序会去读取模型文件（`glTF`文件），如果没找到该模型文件会导致该异常
  * **`解决方法`**：将代码中的模型文件路径指向`./asset/`目录