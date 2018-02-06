#!/usr/bin/env perl

use v5.12;
use strict;
use warnings;

use Mojolicious::Lite;
use DBI;
use POSIX qw(strftime);

my $db_file = $ENV{DB_PATH} // 'db';

my $dbh = DBI->connect("dbi:SQLite:${db_file}", "", "") or die "opening db failed";
$dbh->do("PRAGMA foreign_keys = ON");

helper unix2ts => sub {
	my ($c, $ts) = @_;

	strftime "%F %H:%M:%S", localtime($ts);
};

get '/' => sub {
	my $c = shift;

	my $rows = $dbh->selectall_arrayref("SELECT * FROM lamps ORDER BY name", { Slice => {} });

	$c->stash(lamps => $rows);
	$c->render(template => 'list');
};

get '/edit/:id' => sub {
	my $c = shift;
	my $id = $c->param('id');

	$c->stash(lamp => $id);
	$c->render(template => 'timeline');
};

get '/log/:id/:offset' => {id => 0, offset => 0} => sub {
	my $c = shift;

	my $limit = 15;

	my $filter = "";
	my @bind;

	my $id = $c->param('id');
	if ($id > 0) {
		$filter .= "AND logs.lamp = ? ";
		push @bind, $id;
	}

	my $offset = $c->param('offset');

	my $rows = $dbh->selectall_arrayref(<<"EOF", { Slice => {} }, @bind);
SELECT logs.rowid, logs.time, logs.message, lamps.name
FROM
    logs LEFT JOIN lamps ON logs.lamp = lamps.id
WHERE 1 = 1 ${filter}
ORDER BY time DESC
LIMIT ${limit} OFFSET ${offset}
EOF

	$c->stash(logs => $rows);
	$c->stash(id => $id);

	if (scalar(@$rows) == $limit) {
		$c->stash(next => $offset + 15);
	} else {
		$c->stash(next => -1);
	}

	if ($offset > 0) {
		$c->stash(prev => $offset - 15);
	} else {
		$c->stash(prev => -1);
	}

	$c->render(template => 'log');
};

get '/api/timeline/:id' => sub {
	my $c = shift;
	my $id = $c->param('id');

	my $rows = $dbh->selectall_arrayref("SELECT * FROM timelines WHERE lamp = ? ORDER BY timestamp ASC", { Slice => {} }, $id);

	my @ret;
	foreach my $row (@$rows) {
		push @ret, [$row->{timestamp}, $row->{color}];
	}

	$c->render(json => \@ret);
};

sub update_timeline {
	my ($id, $timeline) = @_;

	$dbh->do("DELETE FROM timelines WHERE lamp = ?", {}, $id);
	for my $point (@$timeline) {
		printf("INSERT INTO timelines(lamp, timestamp, color) VALUES(%d, %d, %d)\n",
		         $id, int(0.5+$point->[0]), int(0.5+$point->[1]));
		$dbh->do('INSERT INTO timelines(lamp, timestamp, color) VALUES(?,?,?)', {},
		         $id, int(0.5+$point->[0]), int(0.5+$point->[1]));
	}
}

post '/api/timeline/:id' => sub {
	my $c = shift;
	my $id = $c->param('id');
	my $json = $c->req->json;

	if (not defined $json or ref($json) ne 'ARRAY') {
		$c->render(text => 'parsing json failed');
		return;
	}

	if (scalar(@$json) > 20) {
		$c->render(text => 'too many points');
		return;
	}

	if ($json->[0][0] != 0) {
		$c->render(text => 'first point needs to be at time 0');
		return;
	}

	for(my $i = 0; $i < scalar(@$json); $i++) {
		my $point = $json->[$i];

		if ($point->[0] < 0 || $point->[0] > 24 * 60 * 60) {
			$c->render(text => "time coordinate of point $i is out of range");
			return;
		}

		if ($point->[1] < 3000 || $point->[1] > 6000) {
			$c->render(text => "color coordinate of point $i is out of range");
			return;
		}
	}

	if ($id =~ /^\d+$/) {
		$dbh->begin_work;
		update_timeline($id, $json);

		$dbh->do('UPDATE lamps SET updated = ? WHERE id = ?', {}, scalar(time), $id);
		$dbh->do('INSERT INTO logs(time, lamp, message) VALUES(?,?,?)', {}, scalar(time), $id, "color timeline updated");

		$dbh->commit;
	} elsif ($id eq 'all') {
		$dbh->begin_work;
		my $lamps = $dbh->selectall_arrayref("SELECT id FROM lamps ORDER BY id", { Slice => {} });

		foreach my $lamp (@$lamps) {
			update_timeline($lamp->{id}, $json);
			$dbh->do('UPDATE lamps SET updated = ? WHERE id = ?', {}, scalar(time), $lamp);
		}

		$dbh->do('INSERT INTO logs(time, lamp, message) VALUES(?,NULL,?)', {}, scalar(time), "all color timelines updated");
		$dbh->commit;
	} else {
		$c->render(text => "invalid lamp specifier");
		return;
	}

	$c->render(text => 'ok');
};

app->start;
