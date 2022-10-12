import Paho from 'paho-mqtt'; //https://www.eclipse.org/paho/files/jsdoc/index.html

import {useState, useContext, useEffect} from 'react';
//import { StatusBar } from 'expo-status-bar';
import {
  StyleSheet,
  Text,
  Button,
  View,
  Image,
  ScrollView,
  Dimensions,
  Alert,
  Modal,
  Pressable,
  TouchableHighlight,
  Platform,
  TouchableOpacitiy,
  TextInput,
} from 'react-native';
import DateTimePicker from '@react-native-community/datetimepicker';
import {DateTimePickerAndroid} from '@react-native-community/datetimepicker';
import React from 'react';
import {SafeAreaProvider, SafeAreaView} from 'react-native-safe-area-context';
import {NavigationContainer} from '@react-navigation/native';
import {NativeStackExample} from './screens/NativeStackExample.tsx';
import {BottomTabNavigator} from './screens/utils/BottomTabNavigator.tsx';
import Icon from 'react-native-vector-icons/AntDesign';
import Ionicons from 'react-native-vector-icons/Ionicons';

import {createBottomTabNavigator} from '@react-navigation/bottom-tabs';

import ImageSlider from './ImageSlider';

import EncryptedStorage from 'react-native-encrypted-storage';

//import {PageContext, PageContextProvider} from './context/pageContext';

const Tab = createBottomTabNavigator();
export const PageContext = React.createContext(null);
export const SettingsContext = React.createContext();

client = new Paho.Client(
  'adeptuscat.ddns.net',
  Number(9001),
  `mqtt-async-test-${parseInt(Math.random() * 100)}`,
);

const images = [
  'https://upload.wikimedia.org/wikipedia/commons/thumb/5/5d/Eclectus_roratus_in_Zoo_Augsburg.jpg/1280px-Eclectus_roratus_in_Zoo_Augsburg.jpg',
  'https://upload.wikimedia.org/wikipedia/commons/e/e0/Green-park.jpg',
  'https://upload.wikimedia.org/wikipedia/commons/2/26/Skyscraper_Los_Angeles_Downtown_2013.jpg',
];

const WIDTH = Dimensions.get('window').width;
const HEIGHT = Dimensions.get('window').height;
var imgKeys = [];
var dotKeys = [];

var once = false;

export default function App() {
  const [connected, setConnected] = useState(false);
  const [name, setName] = useState('katz');

  return (
    <PageContext.Provider value={{connected, setConnected, name, setName}}>
      <Page />
    </PageContext.Provider>
  );
}

const Page = () => {
  var statusString = 'Katzomat Status:';
  const [value, setValue] = useState(0);
  const [status, setStatus] = useState(`${statusString} unkown`);
  //const {connected, setConnected} = useState(PageContext);
  //const {name, setName} = React.useContext(PageContext);
  const context = React.useContext(PageContext);
  const [password, setPassword] = useState('katz!');
  const [domain, setDomain] = useState('adeptuscat.ddns.net');
  const [rotationTime, setRotationTime] = useState(3000);
  const [dir, setDir] = useState(`katzomat/${context.name}`);
  const [img, setImg] = useState('Base64ImgHere');
  const [imgs, setImgs] = useState([
    {
      timestamp: 1661360552, //1661360551
      uri: 'https://upload.wikimedia.org/wikipedia/commons/thumb/5/5d/Eclectus_roratus_in_Zoo_Augsburg.jpg/1280px-Eclectus_roratus_in_Zoo_Augsburg.jpg',
      date: 'fsfasdf',
    },
  ]);
  const [timetable, setTimetable] = useState([]);
  const [statusVerbose, setStatusVerbose] = useState({});

  function formatTime(timestamp) {
    var date = new Date(timestamp * 1000);
    var year = date.getFullYear();
    var month = date.getMonth();
    var day = date.getDay();
    var hours = date.getHours();
    var minutes = '0' + date.getMinutes();
    var seconds = '0' + date.getSeconds();
    var formattedTime =
      day +
      '.' +
      month +
      '.' +
      year +
      ' ' +
      hours +
      ':' +
      minutes.substr(-2) +
      ':' +
      seconds.substr(-2);
    return formattedTime;
  }

  function onMessage(message) {
    if (message.destinationName === `katzomat/${context.name}/status`) {
      setStatus(`${statusString} ${message.payloadString}`);
    }
    if (message.destinationName === `katzomat/${context.name}/status/verbose`) {
      //console.log(message.payloadString);
      var obj = JSON.parse(message.payloadString);
      setStatusVerbose(obj);
      console.log(message.payloadString);

      var dict = {
        time: '',
        history: [],
      };
      dict.time = formatTime(obj.time);
      for (let i = 0; i < obj.history.length; i++) {
        dict.history.push(formatTime(obj.history[i]));
      }
      console.log(dict);
    }
    const str = `katzomat/${context.name}/images`;
    const re = message.destinationName.match(str);
    if (re != null) {
      if (message.destinationName === `katzomat/${context.name}/images/take`) {
        return;
      }
      var obj = JSON.parse(message.payloadString);
      var base64Img = `data:image/png;base64,${obj.value}`;
      var timestamp = parseInt(obj.timestamp, 10);
      //setImg(base64Img);
      for (let i = 0; i < imgs.length; i++) {
        //console.log(timestamp);
        //console.log(imgs[i].timestamp);
        //console.log('next');
        if (imgs[i].timestamp === timestamp) {
          console.log('has img already');
          return;
        }
      }
      setImg(base64Img);
      var date = new Date(timestamp * 1000);
      var year = date.getFullYear();
      var month = date.getMonth();
      var day = date.getDay();
      var hours = date.getHours();
      var minutes = '0' + date.getMinutes();
      var seconds = '0' + date.getSeconds();
      var formattedTime =
        day +
        '.' +
        month +
        '.' +
        year +
        ' ' +
        hours +
        ':' +
        minutes.substr(-2) +
        ':' +
        seconds.substr(-2);
      var arr = imgs;
      arr.push({
        timestamp: timestamp,
        uri: base64Img,
        date: formattedTime,
      });
      arr.sort((a, b) => a.timestamp - b.timestamp);
      //console.log(base64Img);
      //console.log(imgs);
      //console.log(arr);
      setImgs(arr);

      //console.log(imgs);
      //var arr = imgss.append({"uri": "base64Img"});
      //console.log(arr);
      //var imgDict = imgs.append({uri: base64Img});
      //setImgs(imgDict);
      //const dict = {
      //  uri: base64Img,
      //};
      //imgs.append(dict);
    }
    if (message.destinationName === `katzomat/${context.name}/feed`) {
      //console.log(message.payloadString);
    }
    if (message.destinationName === `katzomat/${context.name}/timetable`) {
      console.log(message.payloadString);
      var obj = JSON.parse(message.payloadString);
      var values = obj.values;
      parseTimetable(values);
    }
    if (message.destinationName === `katzomat/${context.name}/settings`) {
      //console.log(message.payloadString);
    }
  }

  function parseTimetable(values) {
    var arr = [];
    for (let i = 0; i < values.length; i++) {
      var recurring = true;
      var days_count = 0;
      var item_text = '';
      for (let ii = 0; ii < values[i].weekdays.length; ii++) {
        days_count += 1;
        var day = values[i].weekdays[ii];
        if (day === 0) {
          item_text += 'Everyday, ';
        }
        if (day === 1) {
          item_text += 'Sun, ';
        }
        if (day === 2) {
          item_text += 'Mon, ';
        }
        if (day === 3) {
          item_text += 'Tue, ';
        }
        if (day === 4) {
          item_text += 'Wed, ';
        }
        if (day === 5) {
          item_text += 'Thu, ';
        }
        if (day === 6) {
          item_text += 'Fri, ';
        }
        if (day === 7) {
          item_text += 'Sat, ';
        }
      }
      if (days_count === 0) {
        recurring = false;
        var date = new Date(values[i].timestamp * 1000);
        var year = date.getFullYear();
        var month = date.getMonth();
        var day = date.getDay();
        var hours = date.getHours();
        var minutes = '0' + date.getMinutes();
        var seconds = '0' + date.getSeconds();
        var formattedTime =
          year +
          '-' +
          ('0' + (month + 1)).slice(-2) +
          '-' +
          ('0' + (day + 1)).slice(-2) +
          ',' +
          ('0' + hours).slice(-2) +
          ':' +
          minutes.substr(-2);
        item_text += formattedTime;
        //if (value[i].timestamp < time.now) {
        //  continue;
        //}
      } else {
        const zeroPad = (num, places) => String(num).padStart(places, '0');
        var hour = zeroPad(values[i].hour, 2);
        var minute = zeroPad(values[i].minute, 2);
        item_text += hour + ':' + minute;
      }
      //console.log(item_text, recurring);
      arr.push({
        text: item_text,
        recurring: recurring,
      });
    }
    console.log(arr);
    setTimetable(arr);
  }

  function onConnected(msg) {
    console.log('Connected!!!!');
    context.setConnected(true);
  }

  function onConnectionLost() {
    context.setConnected(false);
    console.log('lost Connection!');
  }

  function connect() {
    console.log(context);
    console.log(context.setName);
    context.setName('whut');
    console.log('dfasdf');
    console.log('what', context.name);
    client.connect({
      useSSL: true,
      userName: context.name,
      password: 'katz!',
      keepAliveInterval: 2,
      cleanSession: true,
      onSuccess: () => {
        console.log('Connected!');
        client.subscribe(`katzomat/${context.name}/status`);
        client.subscribe(`katzomat/${context.name}/status/verbose`);
        client.subscribe(`katzomat/${context.name}/images/#`);
        client.subscribe(`katzomat/${context.name}/feed`);
        client.subscribe(`katzomat/${context.name}/timetable`);
        client.subscribe(`katzomat/${context.name}/settings`);
        client.onMessageArrived = onMessage;
        client.onConnected = onConnected;
        client.onConnectionLost = onConnectionLost;
      },
      onFailure: () => {
        console.log('Failed to connect!');
      },
    });
  }

  const alert = (flag, data) => {
    if (context.connected === false) {
      connectionAlert();
      return;
    }
    if (
      status === 'Katzomat Status: unkown' ||
      status === 'Katzomat Status: offline'
    ) {
      offlineAlert(flag);
      return;
    } else {
      command(flag, data);
    }
  };

  const offlineAlert = flag =>
    Alert.alert(
      'Katzomat Offline',
      'The Katzomat will the command as soon it goes online.',
      [
        {
          text: 'Cancel',
          onPress: () => console.log('Cancel Pressed'),
          style: 'cancel',
        },
        {text: 'Send it anyway', onPress: () => command(flag)},
      ],
    );

  const command = (flag, data) => {
    console.log(flag);
    switch (flag) {
      case 'refresh':
        refresh();
        break;
      case 'feed':
        feedAlert();
        break;
      case 'addTimetable':
        addTimetable(data);
        break;
    }
  };

  const feedAlert = () =>
    Alert.alert('Feed The Cat', 'Do you want to feed the cat?', [
      {
        text: 'Cancel',
        onPress: () => console.log('Cancel Pressed'),
        style: 'cancel',
      },
      {text: 'OK', onPress: () => feed()},
    ]);

  const connectionAlert = () =>
    Alert.alert('Not connected!', 'Please connect to the KatzoMQTT.', [
      {text: 'Settings', onPress: () => console.log('Go to Settings')},
      {
        text: 'Cancel',
        onPress: () => console.log('Cancel Pressed'),
        style: 'cancel',
      },
      {text: 'OK', onPress: () => connect()},
    ]);

  function addTimetable(data) {
    console.log('add timetable');
    var arr = timetable;
    arr.push({text: 'asdf', recurring: true});
    console.log(arr);
    setTimetable(arr);
  }

  function feed() {
    const message = new Paho.Message('0');
    message.destinationName = `katzomat/${context.name}/feed`;
    client.send(message);
    console.log('sent');
  }

  function refresh() {
    const message = new Paho.Message('0');
    message.destinationName = `katzomat/${context.name}/images/take`;
    client.send(message);
    console.log('sent');
  }

  async function saveAccountSettings(settings) {
    console.log(settings);
    try {
      await EncryptedStorage.setItem(
        'accountSettings',
        JSON.stringify({
          settings,
        }),
      );
      console.log('saveAccountSettings');
      // Congrats! You've just stored your first value!
    } catch (error) {
      // There was an error on the native side
      console.log(error);
    }
  }

  async function loadAccountSettings() {
    try {
      const session = await EncryptedStorage.getItem('accountSettings');

      if (session !== undefined) {
        console.log('wut');
        console.log(session);
        //setDomain(session.domain);
        //setName(session.name);
        //setPassword(session.password);
        // Congrats! You've just retrieved your first value!
      }
    } catch (error) {
      console.log(error);
      // There was an error on the native side
    }
  }

  async function saveDeviceSettings(rotationTime) {
    console.log('saveDeviceSettings');

    if (context.connected === false) {
      connectionAlert();
      return;
    } else {
      const message = new Paho.Message(rotationTime);
      message.destinationName = `katzomat/${context.name}/settings`;
      client.send(message);
    }

    try {
      await EncryptedStorage.setItem(
        'deviceSettings',
        JSON.stringify({
          rotationTime,
        }),
      );
      console.log('saveDeviceSettings');
    } catch (error) {
      console.log(error);
    }
  }

  async function loadDeviceSettings() {
    try {
      const session = await EncryptedStorage.getItem('deviceSettings');

      if (session !== undefined) {
        console.log(session);
        setRotationTime(session);
        // Congrats! You've just retrieved your first value!
      }
    } catch (error) {
      console.log(error);
      // There was an error on the native side
    }
  }

  React.useEffect(() => {
    loadAccountSettings();
    loadDeviceSettings();
  }, []);

  return (
    <SafeAreaProvider>
      <NavigationContainer>
        {/*<NativeStackExample /> */}
        <Tab.Navigator
          screenOptions={{
            tabBarActiveTintColor: 'tomato',
            tabBarInactiveTintColor: 'blue',
            headerTintColor: 'white',
            headerTitleStyle: {
              fontWeight: 'bold',
            },
            headerStyle: {
              backgroundColor: 'red',
            },
          }}>
          <Tab.Screen
            name="HomeTab"
            //component={HomeTabScreen}
            options={{
              tabBarBadge: 3,
              title: 'Home',
              headerTitle: `${status}`,
              tabBarIcon: ({size, color}) => <Icon name={'home'} size={20} />,
            }}>
            {props => (
              <HomeTabScreen
                {...props}
                connect={connect}
                feed={feed}
                refresh={refresh}
                imgs={imgs}
                timetable={timetable}
                alert={alert}
              />
            )}
          </Tab.Screen>
        </Tab.Navigator>
      </NavigationContainer>
    </SafeAreaProvider>
  );
};

interface ChildProps {
  parentFunction: Function;
}

function HomeTabScreen(route) {
  const {connected, setConnected} = React.useContext(PageContext);
  const [imgs, setImgs] = useState([]);
  const [imgActive, setimgActive] = useState(0);
  const [imgsLength, setimgsLength] = useState(0);

  //console.log(route);
  React.useLayoutEffect(() => {
    route.navigation.setOptions({
      //headerTitle: `Count is ${status}`, //use this to make the title changeable
      headerRight: () => (
        <View style={styles.connectButton}>
          <Button
            title={connected ? 'Connected' : 'Log In'}
            color="black"
            onPress={() => route.connect()}
          />
        </View>
      ),
    });
  }, [route, route.navigation, connected]); // ← This `connected` here ensures that the header state is updated

  const onchange = nativeEvent => {
    if (nativeEvent) {
      const slide = Math.ceil(
        nativeEvent.contentOffset.x / nativeEvent.layoutMeasurement.width,
      );
      //console.log("slide");
      //console.log(slide);
      //console.log(imgActive);
      if (slide !== imgActive) {
        setimgActive(slide);
      }
    }
  };

  function scrollToEndOnNewImg(arrLength) {
    if (this._scrollView) {
      if (arrLength !== imgsLength) {
        setimgsLength(arrLength);
        this._scrollView.scrollToEnd({animated: true});
      } else {
        //this._scrollView.scrollTo({y: imgActive * WIDTH});
      }
    }
  }

  return (
    <View style={styles.page}>
      <SafeAreaView style={styles.container}>
        <View style={styles.wrap}>
          <ScrollView
            ref={view => (this._scrollView = view)}
            onScroll={({nativeEvent}) => onchange(nativeEvent)}
            //onScroll={onChange}
            showsHorizontalScrollIndicator={false}
            pagingEnabeled
            horizontal
            style={styles.wrap}>
            {route.imgs.map((e, index) => {
              return (
                <Image
                  key={e.timestamp}
                  resizeMode="contain"
                  style={styles.wrapImg}
                  source={{uri: e.uri}}
                />
              );
            }, scrollToEndOnNewImg(route.imgs.length))}
          </ScrollView>
        </View>
        <View style={styles.wrapDot}>
          {route.imgs.map((e, index) => {
            return (
              <Text
                key={e.timestamp}
                style={imgActive === index ? styles.dotActive : styles.dot}>
                ●
              </Text>
            );
          })}
        </View>
        <View style={styles.wrapDate}>
          <Text>{route.imgs[imgActive].date}</Text>
        </View>
        <View style={styles.button}>
          <Button title={'Feed The Cat'} onPress={() => route.alert('feed')} />
        </View>
        <View style={styles.button}>
          <Button
            title={'Refresh Image'}
            onPress={() => route.alert('refresh')}
          />
        </View>
      </SafeAreaView>
    </View>
  );
}

const styles = StyleSheet.create({
  check: {
    alignSelf: 'center',
  },
  weekdayName: {
    textTransform: 'capitalize',
    fontSize: 16,
  },
  checkBox: {
    width: 25,
    height: 25,
    borderWidth: 2,
    borderColor: 'green',
    marginRight: 5,
  },
  weekdays: {
    flexDirection: 'row',
  },
  options: {
    alignSelf: 'flex-start',
    marginLeft: 50,
  },
  container: {
    flex: 1,
  },
  wrap: {
    width: WIDTH,
    height: HEIGHT * 0.5,
  },
  wrapImg: {
    width: WIDTH,
    height: HEIGHT * 0.5,
  },
  wrapDot: {
    flexDirection: 'row',
    alignSelf: 'center',
  },
  dotActive: {
    margin: 3,
    color: 'black',
  },
  dot: {
    margin: 3,
    color: 'red',
  },
  wrapDate: {
    alignSelf: 'center',
  },
  centeredView: {
    flex: 1,
    justifyContent: 'center',
    alignItems: 'center',
    marginTop: 22,
  },
  modalView: {
    margin: 20,
    backgroundColor: 'blue',
    borderRadius: 20,
    padding: 50,
    alignItems: 'center',
    shadowColor: '#000',
    shadowOffset: {
      width: 0,
      height: 2,
    },
    shadowOpacity: 0.25,
    shadowRadius: 4,
    elevation: 5,
  },
  page: {
    flex: 1,
    justifyContent: 'center',
    alignItems: 'center',
    backgroundColor: '#6ED4C8',
    padding: 30,
  },
  header: {
    headerTintColor: 'black',
    headerTitleStyle: {
      fontWeight: 'bold',
    },
    headerStyle: {
      backgroundColor: 'red',
      paddingHorizontal: 15,
    },
  },
  connectButton: {
    margin: 5,
  },
  button: {
    margin: 10,
    //padding: 20,
  },
  input: {
    alignSelf: 'stretch',
    flexDirection: 'row',
    height: 30,
    margin: 6,
    borderWidth: 1,
    padding: 5,
  },
});
