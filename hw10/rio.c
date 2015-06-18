
ssize_t rio_readn(int fd, void *usrbuf, int n) {
	int nleft, rc;
	nleft = n;
	char *buf = usrbuf;

	while(nleft > 0) {
		if((rc = read(fd, buf, nleft)) < 0) {
			if(errno == EINTR) {
				rc = 0;
			} else {
				return -1;
			}
		} else if(rc == 0) {
			break;
		}
		nleft -= rc;
		buf += rc;
	}
	return n-nleft;
}

ssize_t rio_writen(int fd, void *usrbuf, int n) {
	int nleft, wc;
	nleft = n;
	char *buf = usrbuf;

	while(nleft > 0) {
		if((wc = write(fd, buf, nleft)) <= 0) {
			if(errno == EINTR) {
				wc = 0;
			} else {
				return -1;
			}
		}
		nleft -= wc;
		buf += wc;
	}
	return n;
}

void rio_initreadb(rio_t *rp, int fd) {
	rp->rio_fd = fd;
	rp->rio_cnt = 0;
	rp->rio_bufptr = rp->rio_buf;
}

static ssize_t rio_read(rio_t *rp, void *usrbuf, size_t n) {
	int cnt;
	
	while(rp->rio_cnt <= 0) {
		rp->rio_cnt = read(rp->rio_fd, rp->rio_buf, sizeof(rp->rio_buf));
		if(rp->rio_cnt < 0) {
			if(errno != EINTR) {
				return -1;
			}
		} else if(rp->rio_cnt == 0) {
			return 0;
		} else {
			rp->rio_bufptr = rp->rio_buf;
		}
	}

	cnt = n;
	if(rp->rio_cnt < cnt) {
		cnt = rp->rio_cnt;
	}
	memcopy(usrbuf, rp->rio_bufptr, cnt);
	rp->rio_cnt -= cnt;
	cp->rio_bufptr += cnt;
	return cnt;
}

ssize rio_readlineb(rio_t *rp, void *usrbuf, size_t maxlen) {
	int i, rc;
	char c;
	char *bufp = usrbuf;

	for(i = 0; i < maxlen - 1; i++) {
		if((rc = rio_readb(rp, &c, 1)) == 1) {
			*bufp++ = c;
			if(c == '\n') {
				break;
			}
		} else if(rc == 0) {
			if(i == 0) {
				return 0;
			} else {
				break;
			}
		else {
			return -1;
	}
	*bufp = 0;
	return i + 1;
}

ssize_t rio_readnb(rio_t *rp, void *usrbuf, size_t n) {
	size_t nleft = n;
	ssize_t rc;
	char *bufp = usrbuf;

	while(nleft > 0) {
		if((rc = rio_readb(rp, bufp, nleft) < 0) {
			if(errno == EINTR) {
				rc = 0;
			} else {
				return -1;
			}
		} else if(rc == 0) {
			break;
		}
		nleft -= rc;
		bufp += rc;
	}
	return n - nleft;
}

	
