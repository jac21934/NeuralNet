executable      = net
universe        = standard

periodic_remove = ((CommittedTime - CommittedSuspensionTime) > 600)

error           = errors/error.$(Process)
log             = logs/log.$(Process)
output          = data/out.$(Process)

queue 100