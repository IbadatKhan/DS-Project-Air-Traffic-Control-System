

#include <iostream>
#include <iomanip>
#include <string>
#include <cstring>
#include <fstream>
#include <ctime>
using namespace std;

const int rows = 20;
const int cols = 20;
const int maxplanes = 10;
const int max_nodes = 20;
const int INF = 99999;
char radar[rows][cols];

class plane {
private:
    int id;
    int r;
    int c;

public:
    plane() {
        id = -1;
        r = -1;
        c = -1;
    }

    void setplane(int planeid, int row, int col) {
        id = planeid;
        r = row;
        c = col;
    }

    int getid() {
        return id;
    }

    int getrow() {
        return r;
    }

    int getcol() {
        return c;
    }

    void setposition(int row, int col) {
        r = row;
        c = col;
    }
};


void initializeradar() {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            radar[i][j] = '-';
        }
    }
}


void airport(int r, int c) {
    if (r >= 0 && r < rows && c >= 0 && c < cols) {
        radar[r][c] = 'A';
    }
}


plane planes[maxplanes];
int planecount = 0;
string flightIDForPlane[maxplanes];

void addplane(int r, int c, const string& flightID) {
    if (planecount >= maxplanes) {
        cout << "\033[31m\n[ERROR] Airspace is full! Cannot add more planes.\033[0m\n";
        return;
    }
    if (r < 0 || r >= rows || c < 0 || c >= cols) {
        cout << "\033[32m\n[ERROR] Coordinates (\033[0m" << r << "," << c << "\033[32m) are out of radar bounds.\033[0m\n";
        return;
    }
    if (radar[r][c] != '-' && radar[r][c] != '.') {
        cout << "\033[31m\n[ERROR] Position occupied! Cannot spawn plane on top of an object.\033[0m\n";
        return;
    }

    for (int i = 0; i < planecount; i++) {
        if (flightIDForPlane[i] == flightID) {
            cout << "\n[ERROR] Plane " << flightID << " is already on the Radar (P" << (i + 1) << ")!\n";
            return;
        }
    }

    planes[planecount].setplane(planecount + 1, r, c);
    radar[r][c] = 'P';
    flightIDForPlane[planecount] = flightID;
    planecount++;

    cout << "\033[31m\n[SUCCESS] Plane \033[0m" << flightID << "\033[31m added as P\033[0m" << planecount
        << " at (" << r << "," << c << ")" << endl;
}
void moveplane(int planeid, int newrow, int newcol) {
    int index = planeid - 1;

    if (index < 0 || index >= planecount) {
        cout << "\033[36m\n[ERROR] Plane P" << planeid << " does not exist.\033[0m\n";
        return;
    }

    if (newrow < 0 || newrow >= rows || newcol < 0 || newcol >= cols) {
        cout << "\033[36m\n[ERROR] Coordinates out of bounds.\033[0m\n";
        return;
    }
    if (radar[newrow][newcol] == 'P' || radar[newrow][newcol] == 'A') {
        cout << "\033[36m\n[CRITICAL WARNING] COLLISION DETECTED! Movement Aborted.\033[0m\n";
        return;
    }

    int oldrow = planes[index].getrow();
    int oldcol = planes[index].getcol();

    radar[oldrow][oldcol] = '-';
    planes[index].setposition(newrow, newcol);
    radar[newrow][newcol] = 'P';
    cout << "\033[36m\n[SUCCESS] Plane P\033[0m" << planeid << "\033[36m moved to (\033[0m" << newrow << "," << newcol << ").\n";
}
void display() {
    system("cls");
    cout << "\033[33m================ SKYNET RADAR ================\033[0m" << endl;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            bool isplane = false;

            for (int k = 0; k < planecount; k++) {
                if (planes[k].getrow() == i && planes[k].getcol() == j) {
                    cout << "\033[34mP\033[0m" << planes[k].getid() << " ";
                    isplane = true;
                    break;
                }
            }

            if (!isplane) {
                if (radar[i][j] == 'A') {
                    cout << "\033[36mA  \033[0m";
                }
                else {
                    cout << ".  ";
                }
            }
        }

        cout << endl;
    }
}


class graph {
private:
    int nodes;
    int edges[max_nodes][max_nodes];

public:
    graph() {
        nodes = 0;

        for (int i = 0; i < max_nodes; i++) {
            for (int j = 0; j < max_nodes; j++) {
                edges[i][j] = 9999;
            }
        }
    }

    void addnode() {
        if (nodes < max_nodes) {
            nodes++;
        }
    }

    void initNodes(int n) {
        for (int i = 0; i < n; i++) {
            addnode();
        }
    }

    void addedge(int u, int v, int w) {
        if (u >= 0 && v >= 0) {
            edges[u][v] = w;
        }
    }

    int getnodecount() {
        return nodes;
    }

    void dijkstra(int start, int dist[]) {
        int visited[max_nodes];

        for (int i = 0; i < nodes; i++) {
            dist[i] = 9999;
            visited[i] = 0;
        }

        if (start < 0 || start >= nodes) {
            return;
        }

        dist[start] = 0;

        for (int i = 0; i < nodes - 1; i++) {
            int minDist = 9999;
            int u = -1;

            for (int j = 0; j < nodes; j++) {
                if (!visited[j] && dist[j] < minDist) {
                    minDist = dist[j];
                    u = j;
                }
            }

            if (u == -1) {
                break;
            }

            visited[u] = 1;

            for (int v = 0; v < nodes; v++) {
                if (edges[u][v] != 9999) {
                    if (dist[u] + edges[u][v] < dist[v]) {
                        dist[v] = dist[u] + edges[u][v];
                    }
                }
            }
        }
    }

    void shortestPathAndPrint(int start, int dest) {
        int dist[max_nodes];
        int visited[max_nodes];
        int parent[max_nodes];

        for (int i = 0; i < nodes; i++) {
            dist[i] = 9999;
            visited[i] = 0;
            parent[i] = -1;
        }

        if (start < 0 || start >= nodes || dest < 0 || dest >= nodes) {
            cout << "\033[34mInvalid nodes\033[0m" << endl;
            return;
        }

        dist[start] = 0;

        for (int step = 0; step < nodes; step++) {
            int minDist = 9999;
            int u = -1;

            for (int i = 0; i < nodes; i++) {
                if (!visited[i] && dist[i] < minDist) {
                    minDist = dist[i];
                    u = i;
                }
            }

            if (u == -1) {
                break;
            }

            visited[u] = 1;

            for (int v = 0; v < nodes; v++) {
                if (edges[u][v] != 9999) {
                    int nd = dist[u] + edges[u][v];

                    if (nd < dist[v]) {
                        dist[v] = nd;
                        parent[v] = u;
                    }
                }
            }
        }

        int path[max_nodes];
        int length = 0;
        int crawl = dest;

        while (crawl != -1) {
            path[length++] = crawl;
            crawl = parent[crawl];
        }

        cout << "Start: Node " << path[length - 1] << endl;

        for (int i = length - 1; i > 0; i--) {
            cout << "  |" << endl;
            cout << "  V" << endl;
            cout << "Node " << path[i - 1] << endl;
        }

        cout << "Distance: " << dist[dest] << endl;
    }
};


graph G;


class landingqueue {
private:
    int heapsize;
    int flightid[maxplanes];
    int priority[maxplanes];

public:
    landingqueue() {
        heapsize = 0;
    }

    void swapInt(int& a, int& b) {
        int t = a;
        a = b;
        b = t;
    }

    void heapifyup(int i) {
        while (i > 0) {
            int parent = (i - 1) / 2;

            if (priority[i] < priority[parent]) {
                swapInt(priority[i], priority[parent]);
                swapInt(flightid[i], flightid[parent]);
                i = parent;
            }
            else {
                break;
            }
        }
    }

    void heapifydown(int i) {
        while (true) {
            int left = 2 * i + 1;
            int right = 2 * i + 2;
            int smallest = i;

            if (left < heapsize && priority[left] < priority[smallest]) {
                smallest = left;
            }

            if (right < heapsize && priority[right] < priority[smallest]) {
                smallest = right;
            }

            if (smallest != i) {
                swapInt(priority[i], priority[smallest]);
                swapInt(flightid[i], flightid[smallest]);
                i = smallest;
            }
            else {
                break;
            }
        }
    }

    void insertflight(int id, int pr) {
        if (heapsize >= maxplanes) {
            return;
        }

        flightid[heapsize] = id;
        priority[heapsize] = pr;
        heapsize++;

        heapifyup(heapsize - 1);
    }

    int extractmin() {
        if (heapsize == 0) {
            return -1;
        }

        int id = flightid[0];
        heapsize--;
        priority[0] = priority[heapsize];
        flightid[0] = flightid[heapsize];

        heapifydown(0);

        return id;
    }

    void updatePriority(int id, int newP) {
        for (int i = 0; i < heapsize; i++) {
            if (flightid[i] == id) {
                priority[i] = newP;
                heapifyup(i);
                break;
            }
        }
    }

    void showQueue() {
        if (heapsize == 0) {
            cout << "\033[36mNo landing queue!\033[0m" << endl;
            return;
        }

        cout << "Next: " << flightIDForPlane[flightid[0] - 1] << " [" << priority[0] << "]" << endl;

        for (int i = 1; i < heapsize; i++) {
            cout << i + 1 << ". " << flightIDForPlane[flightid[i] - 1] << " [" << priority[i] << "]" << endl;
        }
    }
};


landingqueue LQ;


const int table_size = 10;

class aircraft {
public:
    string flightid;
    string model;
    int fuel;
    string origin;
    string destination;
    string status;
    aircraft* next;

    aircraft() {
        flightid = "";
        model = "";
        fuel = 0;
        origin = "";
        destination = "";
        status = "";
        next = NULL;
    }

    aircraft(string id, string m, int f, string o, string d, string s) {
        flightid = id;
        model = m;
        fuel = f;
        origin = o;
        destination = d;
        status = s;
        next = NULL;
    }
};


class hashtable {
private:
    aircraft* table[table_size];

    int hashfunction(string key) {
        int sum = 0;

        for (int i = 0; key[i] != '\0'; i++) {
            sum += (int)key[i];
        }

        return sum % table_size;
    }

public:
    hashtable() {
        for (int i = 0; i < table_size; i++) {
            table[i] = NULL;
        }
    }

    void insert(aircraft* A) {

        if (search(A->flightid) != NULL) {
            cout << "\033[33m\n[ERROR] Aircraft \033[0m" << A->flightid << " is already registered!\n";
            return;
        }

        int idx = hashfunction(A->flightid);
        A->next = table[idx];
        table[idx] = A;
        cout << "\033[33m\n[SUCCESS] Registered \033[0m" << A->flightid << endl;
    }

    aircraft* search(string id) {
        int idx = hashfunction(id);
        aircraft* t = table[idx];

        while (t != NULL) {
            if (t->flightid == id) {
                return t;
            }

            t = t->next;
        }

        return NULL;
    }
    void updatestatus(string id, string s) {
        aircraft* a = search(id);

        if (a != NULL) {
            a->status = s;
            cout << "\033[36m\n[SUCCESS] Status updated for \033[0m" << id << " to '" << s << "'.\n";
        }
        else {
            cout << "\n[ERROR] Aircraft Not Found in Registry.\n";
        }
    }

    void save() {
        ofstream f("aircraft_data.txt");

        for (int i = 0; i < table_size; i++) {
            aircraft* t = table[i];

            while (t != NULL) {
                f << t->flightid << "|" << t->model << "|" << t->fuel << "|"
                    << t->origin << "|" << t->destination << "|" << t->status << endl;
                t = t->next;
            }
        }

        f.close();
    }

    void load() {
        ifstream f("aircraft_data.txt");

        if (!f) {
            return;
        }

        string line;

        while (getline(f, line)) {
            if (line.size() == 0) {
                continue;
            }

            string parts[6];
            int p = 0;
            string cur = "";

            for (int i = 0; i < (int)line.size(); i++) {
                if (line[i] == '|') {
                    parts[p++] = cur;
                    cur = "";
                }
                else {
                    cur.push_back(line[i]);
                }
            }

            parts[p] = cur;

            int fuel = 0;

            for (int i = 0; i < (int)parts[2].size(); i++) {
                if (parts[2][i] >= '0' && parts[2][i] <= '9') {
                    fuel = fuel * 10 + (parts[2][i] - '0');
                }
            }

            insert(new aircraft(parts[0], parts[1], fuel, parts[3], parts[4], parts[5]));
        }

        f.close();
    }
};


hashtable Registry;


class lognode {
public:
    string flightid;
    string status;
    int timestamp;
    lognode* left;
    lognode* right;
    int height;

    lognode(string id, string st, int ts) {
        flightid = id;
        status = st;
        timestamp = ts;
        left = NULL;
        right = NULL;
        height = 1;
    }
};


class flightlogavl {
public:
    lognode* root;

    flightlogavl() {
        root = NULL;
    }

    int getheight(lognode* n) {
        if (n == NULL) {
            return 0;
        }

        return n->height;
    }

    int getbalance(lognode* n) {
        if (n == NULL) {
            return 0;
        }

        return getheight(n->left) - getheight(n->right);
    }

    lognode* rightrotate(lognode* y) {
        lognode* x = y->left;
        lognode* T = x->right;

        x->right = y;
        y->left = T;

        int hl = getheight(y->left);
        int hr = getheight(y->right);

        if (hl > hr) {
            y->height = hl + 1;
        }
        else {
            y->height = hr + 1;
        }

        hl = getheight(x->left);
        hr = getheight(x->right);

        if (hl > hr) {
            x->height = hl + 1;
        }
        else {
            x->height = hr + 1;
        }

        return x;
    }

    lognode* leftrotate(lognode* x) {
        lognode* y = x->right;
        lognode* T = y->left;

        y->left = x;
        x->right = T;

        int hl = getheight(x->left);
        int hr = getheight(x->right);

        if (hl > hr) {
            x->height = hl + 1;
        }
        else {
            x->height = hr + 1;
        }

        hl = getheight(y->left);
        hr = getheight(y->right);

        if (hl > hr) {
            y->height = hl + 1;
        }
        else {
            y->height = hr + 1;
        }

        return y;
    }

    lognode* insertnode(lognode* node, string id, string st, int ts) {
        if (node == NULL) {
            return new lognode(id, st, ts);
        }

        if (ts < node->timestamp) {
            node->left = insertnode(node->left, id, st, ts);
        }
        else if (ts > node->timestamp) {
            node->right = insertnode(node->right, id, st, ts);
        }
        else {
            return node;
        }

        int hl = getheight(node->left);
        int hr = getheight(node->right);

        if (hl > hr) {
            node->height = hl + 1;
        }
        else {
            node->height = hr + 1;
        }

        int b = getbalance(node);

        if (b > 1 && ts < node->left->timestamp) {
            return rightrotate(node);
        }

        if (b < -1 && ts > node->right->timestamp) {
            return leftrotate(node);
        }

        if (b > 1 && ts > node->left->timestamp) {
            node->left = leftrotate(node->left);
            return rightrotate(node);
        }

        if (b < -1 && ts < node->right->timestamp) {
            node->right = rightrotate(node->right);
            return leftrotate(node);
        }

        return node;
    }

    void insertlog(string id, string st, int ts) {
        root = insertnode(root, id, st, ts);
    }

    void inorderprint(lognode* n) {
        if (n == NULL) {
            return;
        }

        inorderprint(n->left);
        cout << "\033[36mTimestamp \033[0m" << n->timestamp << "\033[36m Flight \033[0m" << n->flightid << "\033[33m Status \033[0m" << n->status << endl;
        inorderprint(n->right);
    }

    void print() {
        inorderprint(root);
    }

    void inorderSave(lognode* n, ofstream& f) {
        if (n == NULL) {
            return;
        }

        inorderSave(n->left, f);
        f << n->timestamp << "|" << n->flightid << "|" << n->status << endl;
        inorderSave(n->right, f);
    }

    void save() {
        ofstream f("flight_logs.txt");
        inorderSave(root, f);
        f.close();
    }

    void load() {
        ifstream f("flight_logs.txt");

        if (!f) {
            return;
        }

        string line;

        while (getline(f, line)) {
            if (line.size() == 0) {
                continue;
            }

            string parts[3];
            int p = 0;
            string cur = "";

            for (int i = 0; i < (int)line.size(); i++) {
                if (line[i] == '|') {
                    parts[p++] = cur;
                    cur = "";
                }
                else {
                    cur.push_back(line[i]);
                }
            }

            parts[p] = cur;

            int ts = 0;

            for (int i = 0; i < (int)parts[0].size(); i++) {
                if (parts[0][i] >= '0' && parts[0][i] <= '9') {
                    ts = ts * 10 + (parts[0][i] - '0');
                }
            }

            insertlog(parts[1], parts[2], ts);
        }

        f.close();
    }
};


flightlogavl LogAVL;


int main() {
    srand(time(0));
    initializeradar();
    airport(3, 3);
    airport(10, 15);

    Registry.load();
    LogAVL.load();

    G.initNodes(5);
    G.addedge(0, 1, 10);
    G.addedge(1, 2, 5);
    G.addedge(2, 4, 20);
    G.addedge(0, 3, 15);
    G.addedge(3, 4, 10);

    while (true) {
        cout << "\033[32m\n===== SKYNET SYSTEM =====\033[0m\n";
        cout << "\033[33m1. Add Plane\033[0m\n";
        cout << "\033[36m2. Move Plane\033[0m\n";
        cout << "\033[31m3. Display Radar\033[0m\n";
        cout << "\033[33m4. Register Aircraft\033[0m\n";
        cout << "\033[35m5. Search Aircraft\033[0m\n";
        cout << "\033[36m6. Update Aircraft Status\033[0m\n";
        cout << "\033[33m7. Add to Landing Queue\033[0m\n";
        cout << "\033[31m8. Process Landing\033[0m\n";
        cout << "9. Show Landing Queue\n";
        cout << "\033[35m10. Show Flight Logs\033[0m\n";
        cout << "\033[36m11. Save All\033[0m\n";
        cout << "\033[31m12. Find Safe Route (Dijkstra)\033[0m\n";
        cout << "\033[32m13. Exit\033[0m\n";
        cout << "\033[34mEnter: \033[0m";

        int ch;
        cin >> ch;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "\033[32m\n[ERROR] Invalid Menu Option. Please enter a number.\033[0m\n";
            continue;
        }
        cin.ignore(1000, '\n');
        if (ch == 1) {
            int r, c;
            string id;
            cout << "\033[32mRow Col FlightID: \033[0m";
            cin >> r >> c >> id;

            if (cin.fail()) {
                cin.clear(); cin.ignore(1000, '\n');
                cout << "\033[32m\n[ERROR] Invalid Input! Row and Col must be integers.\033[0m\n";
                continue;
            }
            cin.ignore(1000, '\n');
            addplane(r, c, id);
        }
        else if (ch == 2) {
            int id, r, c;
            cout << "\033[31mPlaneID NewRow NewCol: \033[0m";
            cin >> id >> r >> c;
            if (cin.fail()) {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "\033[33m\n[ERROR] Invalid Input! ID, Row, and Col must be integers.\033[0m\n";
                continue;
            }
            cin.ignore(1000, '\n');
            moveplane(id, r, c);
        }
        else if (ch == 3) {
            display();
        }
        else if (ch == 4) {
            string id, m, o, d, s;
            int f;
            cout << "\033[32mFlightID Model Fuel Origin Destination Status: \033[0m";
            cin >> id >> m >> f >> o >> d >> s;

            if (cin.fail()) {
                cin.clear(); cin.ignore(1000, '\n');
                cout << "\033[34m\n[ERROR] Invalid Input! Fuel must be a number.\033[0m\n";
                continue;
            }
            cin.ignore(1000, '\n');
            Registry.insert(new aircraft(id, m, f, o, d, s));
        }
        else if (ch == 5) {
            string id;
            cout << "FlightID: ";
            cin >> id;
            cin.ignore(1000, '\n');
            aircraft* a = Registry.search(id);

            if (a) {
                cout << "\n[FOUND] " << a->flightid << " | " << a->model << " | Fuel: " << a->fuel << "% | " << a->status << endl;
            }
            else {
                cout << "\033[36m\n[ERROR] Aircraft Not Found.\033[0m\n";
            }
        }
        else if (ch == 6) {
            string id, s;
            cout << "\033[33mFlightID NewStatus: \033[0m";
            cin >> id >> s;
            cin.ignore(1000, '\n');
            Registry.updatestatus(id, s);
        }
        else if (ch == 7) {
            int id, pr;
            cout << "\033[31mPlaneID Priority (1=Critical, 2=High, 3=Normal): \033[0m";
            cin >> id >> pr;
            if (cin.fail()) {
                cin.clear(); cin.ignore(1000, '\n');
                cout << "\n[ERROR] Invalid Input! ID and Priority must be integers.\n";
                continue;
            }
            cin.ignore(1000, '\n');
            if (id < 1 || id > planecount) {
                cout << "\033[36m\n[ERROR] Plane P" << id << " does not exist on Radar.\033[0m\n";
                continue;
            }
            if (pr < 1 || pr > 4) {
                cout << "\033[36m\n[ERROR] Priority must be between 1 and 4.\033[0m\n";
                continue;
            }

            LQ.insertflight(id, pr);
            cout << "\033[36m[SUCCESS] Added P" << id << " to Landing Queue.\033[0m\n";
        }
        else if (ch == 8) {
            int landed = LQ.extractmin();

            if (landed != -1) {
                int idx = landed - 1;

                if (idx >= 0 && idx < planecount) {
                    int r = planes[idx].getrow();
                    int c = planes[idx].getcol();
                    radar[r][c] = '-';
                    LogAVL.insertlog(flightIDForPlane[idx], "Landed", rand() % 9999);
                    cout << "\033[31m\n[SUCCESS] Plane " << flightIDForPlane[idx] << " has Landed safely.\033[0m\n";
                }
            }
            else {
                cout << "\033[33m\n[INFO] No planes in landing queue.\033[0m\n";
            }
        }
        else if (ch == 9) {
            LQ.showQueue();
        }
        else if (ch == 10) {
            cout << "\033[35m\n--- FLIGHT LOGS ---\033[0m\n";
            LogAVL.print();
        }
        else if (ch == 11) {
            Registry.save();
            LogAVL.save();
            cout << "\033[32m\n[SUCCESS] System Data Saved.\033[0m\n";
        }
        else if (ch == 12) {
            cout << "\033[32m\n--- AIRSPACE NODES ---\033[0m\n";
            cout << "\033[33m0: JFK Airport\033[0m\n";
            cout << "\033[36m1: Waypoint Alpha\033[0m\n";
            cout << "\033[33m2: Waypoint Bravo\033[0m\n";
            cout << "\033[36m3: Waypoint Charlie\033[0m\n";
            cout << "\033[33m4: LHR Airport\033[0m\n";
            cout << "\033[35m----------------------\033[0m\n";

            int startNode, endNode;
            cout << "\033[36mEnter Start Node ID and Destination Node ID: \033[0m";
            cin >> startNode >> endNode;

            if (cin.fail()) {
                cin.clear(); cin.ignore(1000, '\n');
                cout << "\033[32m\n[ERROR] Nodes must be integers.\033[0m\n";
                continue;
            }
            cin.ignore(1000, '\n');

            if (startNode < 0 || startNode >= G.getnodecount() || endNode < 0 || endNode >= G.getnodecount()) {
                cout << "\033[32m\n[ERROR] Invalid Node IDs. Please check the legend.\033[0m\n";
                continue;
            }

            cout << "\n\033[36mCALCULATING SAFE ROUTE...\033[0m\n";
            G.shortestPathAndPrint(startNode, endNode);
        }
        else if (ch == 13) {
            cout << "\033[32mExiting SkyNet... Goodbye!\033[0m\n";
            break;
        }
        else {
            cout << "\033[31m\n[ERROR] Please select a valid option (1-12).\033[0m\n";
        }
    }
    system("pause");
    return 0;
}