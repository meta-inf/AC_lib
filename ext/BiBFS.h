bool connect (int x1, int y1, int x2, int y2)
{
	int t1 = ++ts, t2 = ++ts;
	while (!que.empty()) que.pop();
	que.push((qNode){x1, y1, t1}); v[x1][y1] = t1;
	que.push((qNode){x2, y2, t2}); v[x2][y2] = t2;
	while (!que.empty())
	{
		qNode cur = que.front(); que.pop();
		for (int i = 0; i < 4; ++i)
		{
			int nx = cur.x + dx[i], ny = cur.y + dy[i];
			if (!valid(nx, ny) || c[nx][ny] != c[cur.x][cur.y]) continue;
			if (v[nx][ny] == cur.ts) continue;
			if (v[nx][ny] == (cur.ts ^ 1)) return true;
			que.push((qNode){nx, ny, cur.ts}); v[nx][ny] = cur.ts;
		}
	}
	return false;
}
