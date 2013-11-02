class SharedMemory {
private:
	int* initMatchShmem();
	int* shm;
	int initSem(key keyVal, int index);
	vector<int> semIds; 
public:
	SharedMemory(int numSemaphores, vector<key>& keys);
	void init();
	int* getShm() const;
	int getSemId(int index) const;
	int getSemIdB() const;
	int getSemIdC() const();	
