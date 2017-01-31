executable		= net
error			= errors/error.$(Process)
log				= logs/log.$(Process)
output			= data/out.$(Process)
universe		= standard

periodic_remove = ((CommittedTime - CommittedSuspensionTime) > 600)

queue 100