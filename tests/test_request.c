void
test_request_parse() {
    struct chttp_request *req;

    req = pipedrequest(&wfd);
    dprintf(wfd, "GET / HTTP/1.1");
}


int
main() {
    test_request_parse();
    return EXIT_SUCCESS;
}
