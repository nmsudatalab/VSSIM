# File: ram_mount.sh
# Date: 2014. 12. 03.
# Author: Jinsoo Yoo (jedisty@hanyang.ac.kr)
# Copyright(c)2014
# Hanyang University, Seoul, Korea
# Embedded Software Systems Laboratory. All right reserved

mkdir rd
chmod 777 rd
sudo mount -t tmpfs -o size=16g tmpfs ./rd
