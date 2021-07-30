# complier

SF(salty fish) complier is a C-developed optimizing complier which is being produced by a group of undergraduate students 
from Huazhong University of Science and Technology.  
It supports the programming language: [Sysy2021](https://gitlab.eduxiji.net/nscscc/compiler2021/-/blob/master/SysY语言定义.pdf),
operating system: Ubuntu 18.04, hardware architecture: ARMv7-32bit.

# To DO List

- [ ] 在SSA IR中增加指向定义所在块的指针，并维护指针
- [ ] 增加释放一个基本块的函数
- [ ] 寄存器分配算法
- [ ] 完成 update_CFG 函数
- [ ] 在 delete_operand 函数中维护 def_use 链
- [x] 在 delete_ir 中维护 def_use 链
