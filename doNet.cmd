executable      = /home/alnwarr/net/net
universe        = standard
local_files     = /dev/urandom

error           = errors/error.$(Process)
log             = logs/log.$(Process)
output          = data/out.$(Process)

queue 100
