class SharedMemory {
private:
	int* initMatchShmem();
	int* shm;
	int initSem(key keyVal, int index);
	vector<int> semIds; 
public:

SharedMemory::SharedMemory(vector<key_t>& keys) {
  	key_t key = keys[0];
  	int flag = SHM_R | SHM_W;

  	shmid = shmget(key, sizeof(struct TradeMessage), flag);
  	
  	if (shmid < 0) {
    	shmid = shmget(key, 1000, flag | IPC_CREAT);
    	if (shmid < 0) {
      	perror("shmget | IPC_CREAT");
    	}
  	}

  	shm = (int *)shmat(shmid, /*addr*/NULL, /*flag*/0); 
  	
  	if (shm == NULL) {
    	perror("shmat");
    	exit(1);
  	}
  	
  	return shm;

}

void
SharedMemory::init() {
}

int* 
SharedMemory::getShm() const {
	return shm;
}

int
SharedMemory::getSemId(int index) const {
	return semIds[index];
}
